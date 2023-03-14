// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCPacket.h"
#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCBundle.h"
#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCMessage.h"
#include "NI2SNetworkCommon.h"



OSCPacketType FNI2SNetworkOSCPacket::GetType(const void* Data, int32 DataLength)
{
	const ANSICHAR* CharData = (const ANSICHAR*)Data;

	if (CharData == nullptr)
	{
		return OSCPacketType::Invalid;
	}

	const ANSICHAR* BundleIdentifier = "#bundle";
	const ANSICHAR* MessageIdentifier = "";		// need to check whether "" is valid or it requires at least /

	const int32 MessageIdentifierLen = FCStringAnsi::Strlen(MessageIdentifier);

	if (DataLength >= FCStringAnsi::Strlen(BundleIdentifier))
	{
		if (FCStringAnsi::Stricmp(CharData, BundleIdentifier) == 0) //-V575
		{
			return OSCPacketType::Bundle;
		}
	}

	if (DataLength >= MessageIdentifierLen)
	{
		if (FCStringAnsi::Strnicmp(CharData, MessageIdentifier, MessageIdentifierLen) == 0) //-V575
		{
			return OSCPacketType::Message;
		}
	}

	return OSCPacketType::Invalid;
}

TSharedPtr<FNI2SNetworkOSCPacket> FNI2SNetworkOSCPacket::CreateFromBuffer(const void * Data, int32 DataLength)
{
	OSCPacketType Type = GetType(Data, DataLength);

	if (Type == OSCPacketType::Bundle)
	{
		return FNI2SNetworkOSCBundle::CreateFromBuffer(Data, DataLength);
	}
	else if (Type == OSCPacketType::Message)
	{
		return FNI2SNetworkOSCMessage::CreateFromBuffer(Data, DataLength);
	}

	return nullptr;
}
