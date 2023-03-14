// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class INI2SNetworkConnection;
class INI2SNetworkPacket;

struct FNI2SNetworkPacketType
{
	FNI2SNetworkPacketType(char A, char B, char C, char D)
		: ID{ A,B,C,D }
	{
		FMemory::Memzero(ID);
	}
	char ID[4];
};


DECLARE_MULTICAST_DELEGATE_OneParam(FNI2SNetworkRouteDelegate, INI2SNetworkPacket&)

template <class ObjectType>
using TNI2SNetworkSharedPtr = TSharedPtr<ObjectType, ESPMode::ThreadSafe>;
