// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NI2SNetwork/Types.h"



class NI2SNETWORK_API FNI2SNetworkDispatchMap
{

public:

	FNI2SNetworkDispatchMap();

	virtual ~FNI2SNetworkDispatchMap() {}

	FDelegateHandle AddRoute(FStringView Path, FNI2SNetworkRouteDelegate::FDelegate Delegate);
	
	void RemoveRoute(FStringView Path, FDelegateHandle DelegateHandle);

	bool	DispatchMessage(INI2SNetworkPacket& Message);

protected:

	TMap<FString, FNI2SNetworkRouteDelegate> DispatchMap;

};