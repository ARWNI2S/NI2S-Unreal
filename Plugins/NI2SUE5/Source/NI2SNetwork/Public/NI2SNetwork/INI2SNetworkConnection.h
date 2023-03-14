// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NI2SNetwork/Types.h"
#include "Containers/StringView.h"

/**
 * Declares the interface for a NI2SNetwork connection
 */
class NI2SNETWORK_API INI2SNetworkConnection
{
public:

	virtual ~INI2SNetworkConnection() {}

	virtual FString GetProtocolName() const = 0;

	virtual TNI2SNetworkSharedPtr<INI2SNetworkPacket> CreatePacket() = 0;

	virtual int SendPacket(const TNI2SNetworkSharedPtr<INI2SNetworkPacket>& Packet) = 0;

	/* Bind a delegate to a message address */
	virtual FDelegateHandle AddRouteDelegate(FStringView Path, FNI2SNetworkRouteDelegate::FDelegate Delegate) = 0;

	/* Remove a delegate handle */
	virtual void RemoveRouteDelegate(FStringView Path, FDelegateHandle& InHandle) = 0;

	/* Set the specified send and receive buffer sizes, if supported */
	virtual void SetBufferSizes(int32 DesiredSendSize, int32 DesiredReceiveSize) = 0;
};