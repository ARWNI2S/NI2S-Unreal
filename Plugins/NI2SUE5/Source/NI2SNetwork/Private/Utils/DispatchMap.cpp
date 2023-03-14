// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Utils/DispatchMap.h"
#include "NI2SNetwork/INI2SNetworkPacket.h"
#include "Containers/StringView.h"


FNI2SNetworkDispatchMap::FNI2SNetworkDispatchMap()
{

}

FDelegateHandle FNI2SNetworkDispatchMap::AddRoute(FStringView InPath, FNI2SNetworkRouteDelegate::FDelegate Delegate)
{
	FString LowerPath = FString(InPath.Len(), InPath.GetData()).ToLower();

	if (DispatchMap.Contains(LowerPath) == false)
	{
		DispatchMap.Add(LowerPath);
	}

	return DispatchMap.FindChecked(LowerPath).Add(Delegate);
}

void FNI2SNetworkDispatchMap::RemoveRoute(FStringView InPath, FDelegateHandle DelegateHandle)
{
	FString LowerPath = FString(InPath.Len(), InPath.GetData()).ToLower();

	auto DelegateList = DispatchMap.Find(LowerPath);

	if (DelegateList != nullptr)
	{
		DelegateList->Remove(DelegateHandle);
	}
}

/*
FNI2SNetworkRouteDelegate& FNI2SNetworkDispatchMap::GetAddressHandler(const TCHAR* Path)
{
	FString LowerPath = FString(Path).ToLower();

	if (DispatchMap.Contains(LowerPath) == false)
	{
		DispatchMap.Add(LowerPath);
	}

	return DispatchMap.FindChecked(LowerPath);
}
*/

bool FNI2SNetworkDispatchMap::DispatchMessage(INI2SNetworkPacket& Message)
{
	FString LowerAddress = Message.GetPath().ToLower();

	bool DidDispatch = true;

	for (const auto& KV : DispatchMap)
	{
		FString LowerPath = KV.Key.ToLower();

		if (LowerAddress.StartsWith(LowerPath))
		{
			KV.Value.Broadcast(Message);
			DidDispatch = true;
		}
	}

	return DidDispatch;
}
