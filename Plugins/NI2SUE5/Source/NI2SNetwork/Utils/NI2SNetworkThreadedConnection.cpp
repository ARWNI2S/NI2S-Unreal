// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Utils/NI2SNetworkThreadedConnection.h"
#include "NI2SNetworkCommon.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "Misc/ScopeLock.h"


FNI2SNetworkThreadedListener::FNI2SNetworkThreadedListener()
{
}

FNI2SNetworkThreadedListener::~FNI2SNetworkThreadedListener()
{
	Stop();
}

bool FNI2SNetworkThreadedListener::IsRunning() const
{
	return bIsRunning;
}

void FNI2SNetworkThreadedListener::Start(TSharedRef<INI2SNetworkSocketConnection> InConnection, FNI2SNetworkListenerDelegate InDelegate)
{
	Connection = InConnection;
	Delegate = InDelegate;

	bIsRunning = true;
	bExitRequested = false;

	FRunnableThread::Create(this, TEXT("FNI2SNetworkSocketThread"), 32 * 1024);
}

uint32 FNI2SNetworkThreadedListener::Run()
{
	while (bExitRequested == false)
	{
		FScopeLock RunningLock(&RunningCS);

		Connection->WaitForConnection(1, [this](TSharedRef<INI2SNetworkSocketConnection> NewConnection) {
			return Delegate.Execute(NewConnection);
		});
	}

	bIsRunning = false;
	return 0;
}

void FNI2SNetworkThreadedListener::Stop()
{
	bExitRequested = true;

	if (IsRunning())
	{
		FScopeLock RunLock(&RunningCS);
	}

	bExitRequested = false;
}
