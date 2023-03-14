// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "INI2SNetworkSocketConnection.h"
#include "Modules/ModuleManager.h"

/**
 *	Main module and factory interface for Backchannel connections
 */
class NI2SNETWORK_API INI2SNetworkTransport : public IModuleInterface
{
public:

	static inline bool IsAvailable(void)
	{
		return Get() != nullptr;
	}

	static inline INI2SNetworkTransport* Get(void)
	{
		return FModuleManager::LoadModulePtr<INI2SNetworkTransport>("NI2SNetwork");
	}

	virtual TSharedPtr<INI2SNetworkSocketConnection> CreateConnection(const int32 Type) = 0;

public:

	static const int TCP;

protected:

	INI2SNetworkTransport() {}
	virtual ~INI2SNetworkTransport() {}
};
