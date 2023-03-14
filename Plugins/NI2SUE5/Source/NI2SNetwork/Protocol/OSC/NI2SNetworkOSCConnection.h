// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "NI2SNetwork/INI2SNetworkConnection.h"
#include "NI2SNetwork/Utils/DispatchMap.h"
#include "HAL/ThreadSafeBool.h"

class INI2SNetworkSocketConnection;
class FNI2SNetworkOSCPacket;

/**
 *	A class that wraps an existing NI2SNetwork connection and provides an OSC-focused interface and
 *	a background thread. Incoming messages are received on a background thread and queued until 
 *	DispatchMessages() is called. Outgoing messages are sent immediately
 */
class NI2SNETWORK_API FNI2SNetworkOSCConnection : FRunnable, public INI2SNetworkConnection
{
public:

	FNI2SNetworkOSCConnection(TSharedRef<INI2SNetworkSocketConnection> InConnection);

	~FNI2SNetworkOSCConnection();

	/**
	 * INI2SNetworkConnection implementation
	 */
public:
	FString GetProtocolName() const override;

	TNI2SNetworkSharedPtr<INI2SNetworkPacket> CreatePacket() override;

	int SendPacket(const TNI2SNetworkSharedPtr<INI2SNetworkPacket>& Packet) override;

	/* Bind a delegate to a message address */
	FDelegateHandle AddRouteDelegate(FStringView Path, FNI2SNetworkRouteDelegate::FDelegate Delegate) override;

	/* Remove a delegate handle */
	void RemoveRouteDelegate(FStringView Path, FDelegateHandle& InHandle) override;

	/* Sets the send and receive buffer sizes*/
	void SetBufferSizes(int32 DesiredSendSize, int32 DesiredReceiveSize) override;

public:

	bool StartReceiveThread();

	// Begin public FRunnable overrides
	virtual void Stop() override;
	// end public FRunnable overrides
	
	/* Returns our connection state as determined by the underlying NI2SNetwork connection */
	bool IsConnected() const;

	/* Returns true if running in the background */
	bool IsThreaded() const;

    /*
        Checks for and dispatches any incoming messages. MaxTime is how long to wait if no data is ready to be read.
        This function is thread-safe and be called from a backfround thread manually or by calling StartReceiveThread()
    */
	void ReceiveAndDispatchMessages(const float MaxTime = 0);

	/* Send the provided OSC packet */
	bool SendPacket(FNI2SNetworkOSCPacket& Packet);

	/* Set options for the specified message path */
	void SetMessageOptions(const TCHAR* Path, int32 MaxQueuedMessages);

	FString GetDescription();

protected:
	// Begin protected FRunnable overrides
	virtual uint32 Run() override;
	// End protected FRunnable overrides

	bool SendPacketData(const void* Data, const int32 DataLen);

	int32 GetMessageLimitForPath(const TCHAR* Path);

	int32 GetMessageCountForPath(const TCHAR* Path);

	void RemoveMessagesWithPath(const TCHAR* Path, const int32 Num = 0);

	void ReceiveMessages(const float MaxTime = 0);

	/* Dispatch all queued messages */
	void DispatchMessages();


protected:

	TSharedPtr<INI2SNetworkSocketConnection>  Connection;

	FNI2SNetworkDispatchMap				DispatchMap;

	TArray<TSharedPtr<FNI2SNetworkOSCPacket>> ReceivedPackets;

	TMap<FString, int32> MessageLimits;

	FThreadSafeBool		ExitRequested;
	FThreadSafeBool		IsRunning;

	FCriticalSection	ReceiveMutex;
	FCriticalSection	SendMutex;
	FCriticalSection	PacketMutex;
	TArray<uint8>		ReceiveBuffer;

	
	/* Time where we'll send a ping if no packets arrive to check the connection is alive*/
	double				PingTime = 2;

	/* Is the connection in an error state */
	bool				HasErrorState = false;

	/* How much data has been received this check? */
	int32				ReceivedDataSize = 0;

	/* How much data do we expect to receive next time? This is for OSC over TCP where the size of a packet is sent, then the packet*/
	int32				ExpectedSizeOfNextPacket = 4;
};
