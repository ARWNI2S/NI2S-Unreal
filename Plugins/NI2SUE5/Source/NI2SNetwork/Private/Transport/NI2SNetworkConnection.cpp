// Copyright Epic Games, Inc. All Rights Reserved.

#include "Transport/NI2SNetworkConnection.h"
#include "NI2SNetwork/Transport/INI2SNetworkTransport.h"
#include "NI2SNetworkCommon.h"
#include "Common/TcpSocketBuilder.h"
#include "CoreGlobals.h"
#include "HAL/IConsoleManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/ScopeLock.h"

DECLARE_STATS_GROUP(TEXT("NI2SNetwork"), STATGROUP_NI2SNetwork, STATCAT_Advanced);

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("BC.PacketsSent/s"), STAT_NI2SNetworkPacketsSent, STATGROUP_NI2SNetwork);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("BC.PacketsRecv/s"), STAT_NI2SNetworkPacketsRecv, STATGROUP_NI2SNetwork);

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("BC.BytesSent/s"), STAT_NI2SNetworkBytesSent, STATGROUP_NI2SNetwork);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("BC.BytesRecv/s"), STAT_NI2SNetworkBytesRecv, STATGROUP_NI2SNetwork);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("BC.Errors/s"), STAT_NI2SNetworkErrors, STATGROUP_NI2SNetwork);


int32 FNI2SNetworkConnection::SendBufferSize = 2 * 1024 * 1024;
int32 FNI2SNetworkConnection::ReceiveBufferSize = 2 * 1024 * 1024;

int32 GNI2SNetworkLogPackets = 0;
static FAutoConsoleVariableRef BCCVarLogPackets(
	TEXT("backchannel.logpackets"), GNI2SNetworkLogPackets,
	TEXT("Logs incoming packets"),
	ECVF_Default);

int32 GNI2SNetworkLogErrors = 1;
static FAutoConsoleVariableRef BCCVarLogErrors(
	TEXT("backchannel.logerrors"), GNI2SNetworkLogErrors,
	TEXT("Logs packet errors"),
	ECVF_Default);

FNI2SNetworkConnection::FNI2SNetworkConnection()
{
	Socket = nullptr;
	IsListener = false;
	TimeSinceStatsSet = 0;
	// Allow the app to override
	GConfig->GetInt(TEXT("NI2SNetwork"), TEXT("SendBufferSize"), SendBufferSize, GEngineIni);
	GConfig->GetInt(TEXT("NI2SNetwork"), TEXT("RecvBufferSize"), ReceiveBufferSize, GEngineIni);
}

FNI2SNetworkConnection::~FNI2SNetworkConnection()
{
	if (Socket)
	{
		Close();
	}
}

/* Todo - Proper stats */
uint32	FNI2SNetworkConnection::GetPacketsReceived() const
{
	return ConnectionStats.PacketsReceived;
}

bool FNI2SNetworkConnection::IsConnected() const
{
	FNI2SNetworkConnection* NonConstThis = const_cast<FNI2SNetworkConnection*>(this);
	FScopeLock Lock(&NonConstThis->SocketMutex);
	return Socket != nullptr && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected;
}

bool FNI2SNetworkConnection::IsListening() const
{
	return IsListener;
}

FString	FNI2SNetworkConnection::GetDescription() const
{
	FNI2SNetworkConnection* NonConstThis = const_cast<FNI2SNetworkConnection*>(this);
	FScopeLock Lock(&NonConstThis->SocketMutex);

	return Socket ? Socket->GetDescription() : TEXT("No Socket");
}

void FNI2SNetworkConnection::Close()
{
	FScopeLock Lock(&SocketMutex);
	if (Socket)
	{
		UE_LOG(LogNI2SNetwork, Log, TEXT("Closing connection %s"), *Socket->GetDescription());
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void FNI2SNetworkConnection::CloseWithError(const TCHAR* Error, FSocket* InSocket)
{
	const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(SE_GET_LAST_ERROR_CODE);

	if (InSocket == nullptr)
	{
		InSocket = Socket;
	}

	FString SockDesc = InSocket != nullptr ? InSocket->GetDescription() : TEXT("(No Socket)");

	UE_LOG(LogNI2SNetwork, Error, TEXT("%s, Err: %s, Socket:%s"), Error, SocketErr, *SockDesc);

	Close();
}

bool FNI2SNetworkConnection::Connect(const TCHAR* InEndPoint)
{
	FScopeLock Lock(&SocketMutex);

	if (IsConnected())
	{
		Close();
	}

	IsAttemptingConnection = true;

	FString LocalEndPoint = InEndPoint;

	FSocket* NewSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("FNI2SNetworkConnection Client Socket"));
	if (NewSocket)
	{
		NewSocket->SetNonBlocking();

		// set buffer sizes
		SetSocketBufferSizes(NewSocket, SendBufferSize, ReceiveBufferSize);

		bool Success = false;

		FIPv4Endpoint EndPointv4;
		// Check for a valid IPv4 address string
		if (FIPv4Endpoint::Parse(LocalEndPoint, EndPointv4))
		{
			Success = NewSocket->Connect(*EndPointv4.ToInternetAddr());
		}

		if (!Success)
		{
			ESocketErrors LastErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();

			if (LastErr == SE_EINPROGRESS || LastErr == SE_EWOULDBLOCK)
			{
				Success = true;
			}
			else
			{
				UE_LOG(LogNI2SNetwork, Log, TEXT("Connect failed with error code (%d) error (%s)"), LastErr, ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(LastErr));
			}
		}

		if (Success)
		{
			UE_LOG(LogNI2SNetwork, Log, TEXT("Opening connection to %s (localport: %d)"), *NewSocket->GetDescription(), NewSocket->GetPortNo());
			Attach(NewSocket);
		}
		else
		{

			CloseWithError(*FString::Printf(TEXT("Failed to open connection to %s."), InEndPoint), NewSocket);
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(NewSocket);
		}
	}

	return Socket != nullptr;
}

// public version that is applied to our socket
void FNI2SNetworkConnection::SetBufferSizes(int32 DesiredSendSize, int32 DesiredReceiveSize)
{
	// Save these for any future connections
	SendBufferSize = FMath::Max(DesiredSendSize, SendBufferSize);
	ReceiveBufferSize = FMath::Max(DesiredReceiveSize, ReceiveBufferSize);

	// If we have a socket, apply it now
	if (Socket != nullptr)
	{
		SetSocketBufferSizes(Socket, SendBufferSize, SendBufferSize);
	}
}

void FNI2SNetworkConnection::SetSocketBufferSizes(FSocket* NewSocket, int32 DesiredSendSize, int32 DesiredReceiveSize)
{
	int32 AllocatedSendSize = 0;
	int32 AllocatedReceiveSize = 0;

	int32 RequestedSendSize = DesiredSendSize;
	int32 RequestedReceiveSize = DesiredReceiveSize;
	bool bWasSet = false;
		
	// Send Buffer
	while (AllocatedSendSize < RequestedSendSize && !bWasSet)
	{
		// note - it's possible for AllocatedSize to change and bWasSet = false if the socket has already had a buffer
		// size set that's the mac supported.
		bWasSet = NewSocket->SetSendBufferSize(RequestedSendSize, AllocatedSendSize);

		// If we didn't get what we want assume failure could mean
		// no change (unsupported size), an OS default, or an OS max so
		// try again if its less than 50% of what we asked for
		if (!bWasSet && AllocatedSendSize < RequestedSendSize)
		{
			RequestedSendSize = RequestedSendSize / 2;
		}
	}
	
	if (AllocatedSendSize != DesiredSendSize)
	{
		UE_LOG(LogNI2SNetwork, Warning, TEXT("Wanted send buffer of %d for %s but OS only allowed %d"), DesiredSendSize, *NewSocket->GetDescription(), AllocatedSendSize);
	}
	else
	{
		UE_LOG(LogNI2SNetwork, Log, TEXT("Set send buffer to %d bytes for %s"), AllocatedSendSize, *NewSocket->GetDescription());
	}
	
	bWasSet = false;

	// Set Receive buffer
	while (AllocatedReceiveSize < RequestedReceiveSize && !bWasSet)
	{
		bWasSet = NewSocket->SetReceiveBufferSize(RequestedReceiveSize, AllocatedReceiveSize);
		
		// If we didn't get what we want assume failure could mean
		// no change (unsupported size), an OS default, or an OS max so
		// try again if its less than 50% of what we asked for
		if (!bWasSet && AllocatedReceiveSize < RequestedReceiveSize)
		{
			RequestedReceiveSize = RequestedReceiveSize / 2;
		}
	}
	
	if (AllocatedReceiveSize != DesiredReceiveSize)
	{
		UE_LOG(LogNI2SNetwork, Warning, TEXT("Wanted receive buffer of %d for %s but OS only allowed %d"), DesiredReceiveSize, *NewSocket->GetDescription(), AllocatedReceiveSize);
	}
	else
	{
		UE_LOG(LogNI2SNetwork, Log, TEXT("Set receive buffer to %d bytes for %s"), AllocatedSendSize, *NewSocket->GetDescription());
	}
}

bool FNI2SNetworkConnection::Listen(const int16 Port)
{
	FScopeLock Lock(&SocketMutex);

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	FSocket* NewSocket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("FNI2SNetworkConnection Client Socket"));
	if (NewSocket != nullptr)
	{
		FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);

		bool Error = !NewSocket->SetReuseAddr(true);

		if (!Error)
		{
			Error = !NewSocket->SetRecvErr();
		}

		if (!Error)
		{
			Error = !NewSocket->SetNonBlocking();
		}


		if (!Error)
		{
			Error = !NewSocket->Bind(*Endpoint.ToInternetAddr());
		}

		if (!Error)
		{
			Error = !NewSocket->Listen(8);
		}

		if (Error)
		{
			const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(SE_GET_LAST_ERROR_CODE);
			GLog->Logf(TEXT("Failed to create the listen socket as configured. %s"), SocketErr);

			SocketSubsystem->DestroySocket(NewSocket);

			NewSocket = nullptr;
		}
	}

	if (NewSocket == nullptr)
	{
		const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(SE_GET_LAST_ERROR_CODE);

		UE_LOG(LogNI2SNetwork, Error, TEXT("Failed to open socket on port %d. Err: %s"), Port, SocketErr);
		CloseWithError(*FString::Printf(TEXT("Failed to start listening on port %d"), Port));
	}
	else
	{
		UE_LOG(LogNI2SNetwork, Log, TEXT("Listening on %s (localport: %d)"), *NewSocket->GetDescription(), NewSocket->GetPortNo());
		Attach(NewSocket);
		IsListener = true;
	}

	return NewSocket != nullptr;
}

bool FNI2SNetworkConnection::WaitForConnection(double InTimeout, TFunction<bool(TSharedRef<INI2SNetworkSocketConnection>)> InDelegate)
{
	FScopeLock Lock(&SocketMutex);

	if (!Socket)
	{
		UE_LOG(LogNI2SNetwork, Error, TEXT("Connection has no socket. Call Listen/Connect before WaitForConnection"));
		return false;
	}

	FTimespan SleepTime = FTimespan(0, 0, InTimeout);

	// handle incoming connections

	bool CheckSucceeded = false;
	bool HasConnection = false;

	if (IsListener)
	{
		CheckSucceeded = Socket->WaitForPendingConnection(HasConnection, SleepTime);
	}
	else
	{
		ESocketConnectionState State = Socket->GetConnectionState();

		if (State == ESocketConnectionState::SCS_ConnectionError)
		{
			ESocketErrors Err = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();

			if (Err == ESocketErrors::SE_NO_ERROR)
			{
				CheckSucceeded = true;
			}
			else
			{
				const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(Err);
				UE_LOG(LogNI2SNetwork, Warning, TEXT("Socket has error %s"), SocketErr);
			}
		}
		else
		{
			CheckSucceeded = true;
			HasConnection = Socket->Wait(ESocketWaitConditions::WaitForWrite, SleepTime);
		}
	}

	if (CheckSucceeded)
	{
		if (HasConnection)
		{
			UE_LOG(LogNI2SNetwork, Log, TEXT("Found connection on %s"), *Socket->GetDescription());

			if (IsListener == false)
			{
				InDelegate(AsShared());
			}
			else
			{
				TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

				FSocket* ConnectionSocket = Socket->Accept(*RemoteAddress, TEXT("RemoteConnection"));

				if (ConnectionSocket != nullptr)
				{
					// Each platform can inherit different socket options from the listen socket so set ours again
					{
						ConnectionSocket->SetNonBlocking();

						// set buffer sizes
						SetSocketBufferSizes(ConnectionSocket, SendBufferSize, ReceiveBufferSize);
					}

					TSharedRef<FNI2SNetworkConnection> BCConnection = MakeShareable(new FNI2SNetworkConnection);
					BCConnection->Attach(ConnectionSocket);

					if (InDelegate(BCConnection) == false)
					{
						UE_LOG(LogNI2SNetwork, Warning, TEXT("Calling code rejected connection on %s"), *Socket->GetDescription());
						BCConnection->Close();
					}
					else
					{
						UE_LOG(LogNI2SNetwork, Log, TEXT("Accepted connection from %s on %s"), *RemoteAddress->ToString(true),  *Socket->GetDescription());
					}
				}
			}
		}
	}
	else
	{
		CloseWithError(TEXT("Connection Check Failed"));
	}

	return CheckSucceeded;
}

bool FNI2SNetworkConnection::Attach(FSocket* InSocket)
{
	FScopeLock Lock(&SocketMutex);

	check(Socket == nullptr);

	Socket = InSocket;
	return true;
}


int32 FNI2SNetworkConnection::SendData(const void* InData, const int32 InSize)
{
	FScopeLock Lock(&SocketMutex);
	if (!Socket)
	{
		return -1;
	}

	int32 BytesSent(0);
	Socket->Send((const uint8*)InData, InSize, BytesSent);

	ResetStatsIfTime();

	if (BytesSent == -1)
	{
		if (GNI2SNetworkLogErrors)
		{
			ESocketErrors LastError = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
			const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(LastError);
			UE_CLOG(GNI2SNetworkLogErrors, LogNI2SNetwork, Error, TEXT("Failed to send %d bytes of data to %s. Err: %s"), InSize, *GetDescription(), SocketErr);
		}

		ConnectionStats.Errors++;
		ConnectionStats.LastErrorTime = FPlatformTime::Seconds();
	}
	else
	{
		ConnectionStats.PacketsSent++;
		ConnectionStats.BytesSent += BytesSent;
		ConnectionStats.LastSuccessTime = FPlatformTime::Seconds();
		ConnectionStats.LastSendTime = ConnectionStats.LastSuccessTime;

		UE_CLOG(GNI2SNetworkLogPackets, LogNI2SNetwork, Log, TEXT("Sent %d bytes of data"), BytesSent);
	}
	return BytesSent;
}

int32 FNI2SNetworkConnection::ReceiveData(void* OutBuffer, const int32 BufferSize)
{
	FScopeLock Lock(&SocketMutex);
	if (!Socket)
	{
		return 0;
	}

	int32 BytesRead(0);
	Socket->Recv((uint8*)OutBuffer, BufferSize, BytesRead, ESocketReceiveFlags::None);	

	ResetStatsIfTime();

	// todo - close connection on certain errors
	if (BytesRead > 0)
	{
		ConnectionStats.PacketsReceived++;
		ConnectionStats.BytesReceived += BytesRead;
		ConnectionStats.LastSuccessTime = FPlatformTime::Seconds();
		ConnectionStats.LastReceiveTime = ConnectionStats.LastSuccessTime;
		UE_CLOG(GNI2SNetworkLogPackets, LogNI2SNetwork, Log, TEXT("Received %d bytes of data"), BytesRead);
	}
	else if (BytesRead < 0)
	{
		// note - FSocket consumes WOULDBLOCk errors so there's not a lot to do here..
	}

	return BytesRead;
}

void FNI2SNetworkConnection::ResetStatsIfTime()
{
	const double TimeNow = FPlatformTime::Seconds();

	if (TimeNow - TimeSinceStatsSet  >= 1.0)
	{
		// stats reflect the last second
		int PacketsReceived = ConnectionStats.PacketsReceived - LastStats.PacketsReceived;
		int BytesReceived = ConnectionStats.BytesReceived - LastStats.BytesReceived;
		int PacketsSent = ConnectionStats.PacketsSent - LastStats.PacketsSent;
		int BytesSent = ConnectionStats.BytesSent - LastStats.BytesSent;
		int Errors = ConnectionStats.Errors - LastStats.Errors;

		SET_DWORD_STAT(STAT_NI2SNetworkPacketsRecv, PacketsReceived);
		SET_DWORD_STAT(STAT_NI2SNetworkBytesRecv, BytesReceived);
		SET_DWORD_STAT(STAT_NI2SNetworkPacketsSent, PacketsSent);
		SET_DWORD_STAT(STAT_NI2SNetworkBytesSent, BytesSent);
		SET_DWORD_STAT(STAT_NI2SNetworkErrors, Errors);

		LastStats = ConnectionStats;
	}
}
