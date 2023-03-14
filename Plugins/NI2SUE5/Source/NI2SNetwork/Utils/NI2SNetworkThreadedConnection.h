// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NI2SNetwork/Transport/INI2SNetworkSocketConnection.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"

class FSocket;

DECLARE_DELEGATE_RetVal_OneParam(bool, FNI2SNetworkListenerDelegate, TSharedRef<INI2SNetworkSocketConnection>);

/**
 * NI2SNetworkClient implementation.
 */
class NI2SNETWORK_API FNI2SNetworkThreadedListener : public FRunnable, public TSharedFromThis<FNI2SNetworkThreadedListener>
{
public:

	FNI2SNetworkThreadedListener();
	~FNI2SNetworkThreadedListener();

	void Start(TSharedRef<INI2SNetworkSocketConnection> InConnection, FNI2SNetworkListenerDelegate InDelegate);

	virtual void Stop() override;

	bool IsRunning() const;

protected:

	virtual uint32 Run() override;

private:

	TSharedPtr<INI2SNetworkSocketConnection>		Connection;
	FNI2SNetworkListenerDelegate			Delegate;
	
	FThreadSafeBool							bExitRequested;
	FThreadSafeBool							bIsRunning;
	FCriticalSection						RunningCS;
};
