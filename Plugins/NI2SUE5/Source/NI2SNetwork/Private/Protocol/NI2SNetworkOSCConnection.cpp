// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCConnection.h"
#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSC.h"
#include "NI2SNetwork/Transport/INI2SNetworkTransport.h"
#include "NI2SNetworkCommon.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Sockets.h"

FNI2SNetworkOSCConnection::FNI2SNetworkOSCConnection(TSharedRef<INI2SNetworkSocketConnection> InConnection)
	: Connection(InConnection)
{
	const int32 kDefaultBufferSize = 4096;
	ReceiveBuffer.AddUninitialized(kDefaultBufferSize);
}

FNI2SNetworkOSCConnection::~FNI2SNetworkOSCConnection()
{
	UE_LOG(LogNI2SNetwork, Verbose, TEXT("Destroying OSC Connection to %s"), *GetDescription());
	if (IsRunning)
	{
		Stop();
	}
}

FString FNI2SNetworkOSCConnection::GetProtocolName() const
{
	return TEXT("BOSC");
}

TNI2SNetworkSharedPtr<INI2SNetworkPacket> FNI2SNetworkOSCConnection::CreatePacket()
{
	return MakeShared<FNI2SNetworkOSCMessage, ESPMode::ThreadSafe>(OSCPacketMode::Write);
}

int FNI2SNetworkOSCConnection::SendPacket(const TNI2SNetworkSharedPtr<INI2SNetworkPacket>& Packet)
{
	FNI2SNetworkOSCMessage* Message = static_cast<FNI2SNetworkOSCMessage*>(Packet.Get());

	if (!SendPacket(*Message))
	{
		return -1;
	}

	return 0;
}


/* Bind a delegate to a message address */
FDelegateHandle FNI2SNetworkOSCConnection::AddRouteDelegate(FStringView Path, FNI2SNetworkRouteDelegate::FDelegate Delegate)
{
	FScopeLock Lock(&PacketMutex);
	return DispatchMap.AddRoute(Path, Delegate);
}

/* Remove a delegate handle */
void FNI2SNetworkOSCConnection::RemoveRouteDelegate(FStringView Path, FDelegateHandle& InHandle)
{
	FScopeLock Lock(&PacketMutex);
	DispatchMap.RemoveRoute(Path, InHandle);
}

void FNI2SNetworkOSCConnection::SetBufferSizes(int32 DesiredSendSize, int32 DesiredReceiveSize)
{
	if (Connection.IsValid())
	{
		Connection->SetBufferSizes(DesiredSendSize, DesiredReceiveSize);
	}
}


/*
FDelegateHandle FNI2SNetworkOSCConnection::AddMessageHandler(const TCHAR* Path, FNI2SNetworkDispatchDelegate::FDelegate Delegate)
{
	FScopeLock Lock(&PacketMutex);
	return LegacyDispatchMap.GetAddressHandler(Path).Add(Delegate);
}


void FNI2SNetworkOSCConnection::RemoveMessageHandler(const TCHAR* Path, FDelegateHandle& InHandle)
{
	LegacyDispatchMap.GetAddressHandler(Path).Remove(InHandle);
	InHandle.Reset();
}*/

void FNI2SNetworkOSCConnection::ReceiveAndDispatchMessages(const float MaxTime /*= 0*/)
{
	ReceiveMessages(MaxTime);
	DispatchMessages();
}

void FNI2SNetworkOSCConnection::ReceiveMessages(const float MaxTime /*= 0*/)
{
	// Cap packets at 128MB.
	const int kMaxPacketSize = 128 * 1024 * 1024;

	const double StartTime = FPlatformTime::Seconds();
    
    //UE_LOG(LogNI2SNetwork, Log, TEXT("Starting Packet Check at %.02f"), StartTime);

	bool KeepReceiving = false;

	int PacketsReceived = 0;

	do
	{
		FScopeLock Lock(&ReceiveMutex);

		Connection->GetSocket()->Wait(ESocketWaitConditions::WaitForRead, FTimespan(0, 0, MaxTime));

		int32 Received = Connection->ReceiveData(ReceiveBuffer.GetData() + ReceivedDataSize, ExpectedSizeOfNextPacket - ReceivedDataSize);

		if (Received > 0)
		{
			ReceivedDataSize += Received;

			if (ReceivedDataSize == ExpectedSizeOfNextPacket)
			{
				// reset this
				ReceivedDataSize = 0;

				if (ExpectedSizeOfNextPacket == 4)
				{
					int32 Size(0);
					FMemory::Memcpy(&Size, ReceiveBuffer.GetData(), sizeof(int32));

					if (Size >= 0 && Size <= kMaxPacketSize)
					{
						if (Size > ReceiveBuffer.Num())
						{
							ReceiveBuffer.AddUninitialized(Size - ReceiveBuffer.Num());
						}
						ExpectedSizeOfNextPacket = Size;
					}
					else
					{
						// if this is abnormally large it's likely a malformed packet so just reject it. We have to disconnect because we've no
                        // idea where the next valid packet in the stream is.
						UE_LOG(LogNI2SNetwork, Error,
						       TEXT("Received packet of %d bytes that was out of the range [0,%d]. Assuming data is malformed and disconnecring"),
						       Size, kMaxPacketSize);
						HasErrorState = true;
						ExpectedSizeOfNextPacket = 4;
					}
				}
				else
				{
					// read packet
					TSharedPtr<FNI2SNetworkOSCPacket> Packet = FNI2SNetworkOSCPacket::CreateFromBuffer(ReceiveBuffer.GetData(), ExpectedSizeOfNextPacket);

					if (Packet.IsValid())
					{
                        FScopeLock PacketLock(&PacketMutex);
                        
						bool bAddPacket = true;

						if (Packet->GetType() == OSCPacketType::Message)
						{
							auto MsgPacket = StaticCastSharedPtr<FNI2SNetworkOSCMessage>(Packet);

							const FString NewAddress = MsgPacket->GetPath();

							UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Received message to %s (tags:%s, size:%d)"), *NewAddress, *MsgPacket->GetTags(), ExpectedSizeOfNextPacket);

							int32 CurrentCount = GetMessageCountForPath(*NewAddress);

							int32 MaxMessages = GetMessageLimitForPath(*NewAddress);

							if (CurrentCount > 0)
							{
								UE_CLOG(GNI2SNetworkLogPackets, LogNI2SNetwork, Log, TEXT("%s has %d unprocessed messages"), *NewAddress, CurrentCount + 1);

								if (MaxMessages > 0 && CurrentCount >= MaxMessages)
								{
									UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Discarding old messages due to limit of %d"), MaxMessages);
									RemoveMessagesWithPath(*NewAddress, 1);
								}
							}
						}
						else
						{
							UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Received #bundle of %d bytes"), ExpectedSizeOfNextPacket);
						}

						ReceivedPackets.Add(Packet);
					}

					ExpectedSizeOfNextPacket = 4;
					PacketsReceived++;
				}
			}
		}

		const double ElapsedTime = FPlatformTime::Seconds() - StartTime;

        // keep receiving until we run out of time, unless we received a packet
		KeepReceiving = ElapsedTime < MaxTime && PacketsReceived == 0 && HasErrorState == false;

	} while (KeepReceiving);
    
    const double ElapsedTime = FPlatformTime::Seconds() - StartTime;
    UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Received %d packets in %.03f secs at %.03f"), PacketsReceived, ElapsedTime, FPlatformTime::Seconds());
}

void FNI2SNetworkOSCConnection::DispatchMessages()
{
	FScopeLock Lock(&PacketMutex);

	for (auto& Packet : ReceivedPackets)
	{
		if (Packet->GetType() == OSCPacketType::Message)
		{
			TSharedPtr<FNI2SNetworkOSCMessage> Msg = StaticCastSharedPtr<FNI2SNetworkOSCMessage>(Packet);

			UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Dispatching %s to handlers"), *Msg->GetPath());

			if (!DispatchMap.DispatchMessage(*Msg.Get()))
			{
				UE_LOG(LogNI2SNetwork, Log, TEXT("Failed to displatch message to %s. No handler?"), *Msg->GetPath());
			}
			Msg->ResetRead();
		}
	}

	ReceivedPackets.Empty();
}

bool FNI2SNetworkOSCConnection::StartReceiveThread()
{
	check(IsRunning == false);

	ExitRequested = false;

	// todo- expose this priority
	FRunnableThread* Thread = FRunnableThread::Create(this, TEXT("OSCHostConnection"), 1024 * 1024, TPri_AboveNormal);

	if (Thread)
	{
		IsRunning = true;
	}

	UE_LOG(LogNI2SNetwork, Verbose, TEXT("Started OSC Connection to %s"), *GetDescription());

	return Thread != nullptr;
}

/* Returns true if running in the background */
bool FNI2SNetworkOSCConnection::IsThreaded() const
{
	return IsRunning;
}

uint32 FNI2SNetworkOSCConnection::Run()
{
    const float kMaxReceiveTime = 1;
	const int32 kDefaultBufferSize = 4096;

	TArray<uint8> Buffer;
	Buffer.AddUninitialized(kDefaultBufferSize);

	UE_LOG(LogNI2SNetwork, Verbose, TEXT("OSC Connection to %s is Running"), *Connection->GetDescription());

	while (ExitRequested == false)
	{
        // This call itself will yield to the OS while waiting for recv so while this looks like a spinloop
        // it really isn't :)
        ReceiveAndDispatchMessages(kMaxReceiveTime);        
	}

	UE_LOG(LogNI2SNetwork, Verbose, TEXT("OSC Connection to %s is exiting."), *Connection->GetDescription());
	IsRunning = false;
	return 0;
}

void FNI2SNetworkOSCConnection::Stop()
{
	if (IsRunning)
	{
		UE_LOG(LogNI2SNetwork, Verbose, TEXT("Requesting OSC Connection to stop.."));

		ExitRequested = true;

		while (IsRunning)
		{
			FPlatformProcess::SleepNoStats(0.01);
		}
	}

	UE_LOG(LogNI2SNetwork, Verbose, TEXT("OSC Connection is stopped"));
	Connection = nullptr;
}

bool FNI2SNetworkOSCConnection::IsConnected() const
{
	return Connection->IsConnected() && (HasErrorState == false);
}

bool FNI2SNetworkOSCConnection::SendPacket(FNI2SNetworkOSCPacket& Packet)
{
	FNI2SNetworkOSCMessage* MsgPacket = (FNI2SNetworkOSCMessage*)&Packet;

	// todo : differentiate between UDP & TCP : for now we assume TCP which requires
	// the buffer size at the start of the array
	
	TArray<uint8> Data;

	// This int32 will hold the number of bytes added to the array by Packet.WriteToBuffer()
	Data.AddUninitialized(sizeof(int32));

	Packet.WriteToBuffer(Data);

	// Calculate the number of bytes added by the above call.
	const int32 BufferSize = Data.Num() - sizeof(int32);

	// Set the first 4 bytes to the buffer length
	FMemory::Memcpy(Data.GetData(), (void*)&BufferSize, sizeof(int32));

	UE_LOG(LogNI2SNetwork, VeryVerbose, TEXT("Sent message to %s (tags:%s, size:%d)"), *MsgPacket->GetPath(), *MsgPacket->GetTags(), Data.Num());

	return SendPacketData(Data.GetData(), Data.Num());
}


bool FNI2SNetworkOSCConnection::SendPacketData(const void* Data, const int32 DataLen)
{
	FScopeLock Lock(&SendMutex);

	if (!IsConnected())
	{
		return false;
	}

	int32 Sent = 0;

	if (DataLen > 0)
	{
		ANSICHAR* RawData = (ANSICHAR*)Data;
		check(FCStringAnsi::Strlen(RawData) < 64);
		Sent = Connection->SendData(Data, DataLen);
	}

	return Sent > 0;
}

FString FNI2SNetworkOSCConnection::GetDescription()
{
	return FString::Printf(TEXT("OSCConnection to %s"), *Connection->GetDescription());
}

void FNI2SNetworkOSCConnection::SetMessageOptions(const TCHAR* Path, int32 MaxQueuedMessages)
{
	FScopeLock Lock(&PacketMutex);
	MessageLimits.FindOrAdd(Path) = MaxQueuedMessages;
}

int32 FNI2SNetworkOSCConnection::GetMessageCountForPath(const TCHAR* Path)
{
	FScopeLock Lock(&PacketMutex);
	
	int32 Count = 0;

	for (auto& Packet : ReceivedPackets)
	{
		if (Packet->GetType() == OSCPacketType::Message)
		{
			auto Msg = StaticCastSharedPtr<FNI2SNetworkOSCMessage>(Packet);

			if (Msg->GetPath() == Path)
			{
				Count++;
			}
		}
	}

	return Count;
}


int32 FNI2SNetworkOSCConnection::GetMessageLimitForPath(const TCHAR* InPath)
{
	FScopeLock Lock(&PacketMutex);

	FString Path = InPath;

	if (Path.EndsWith(TEXT("*")))
	{
		Path.LeftChopInline(1);
	}

	// todo - search for vest match, not first match
	for (auto KV : MessageLimits)
	{
		const FString& Key = KV.Key;
		if (Path.StartsWith(Key))
		{
			return KV.Value;
		}
	}

	return -1;
}

void FNI2SNetworkOSCConnection::RemoveMessagesWithPath(const TCHAR* Path, const int32 Num /*= 0*/)
{
	FScopeLock Lock(&PacketMutex);

	auto It = ReceivedPackets.CreateIterator();

	int32 RemovedCount = 0;

	while (It)
	{
		auto Packet = *It;
		bool bRemove = false;

		if (Packet->GetType() == OSCPacketType::Message)
		{
			TSharedPtr<FNI2SNetworkOSCMessage> Msg = StaticCastSharedPtr<FNI2SNetworkOSCMessage>(Packet);

			if (Msg->GetPath() == Path)
			{
				bRemove = true;
			}
		}

		if (bRemove)
		{
			It.RemoveCurrent();
			RemovedCount++;

			if (Num > 0 && RemovedCount == Num)
			{
				break;
			}
		}
		else
		{
			It++;
		}
	}
}

