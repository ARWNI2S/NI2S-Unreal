// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCPacket.h"


class FNI2SNetworkOSCBundle : public FNI2SNetworkOSCPacket
{
public:

	FNI2SNetworkOSCBundle(OSCPacketMode InMode);
	virtual ~FNI2SNetworkOSCBundle();

	FNI2SNetworkOSCBundle(FNI2SNetworkOSCBundle&& RHS);

	FNI2SNetworkOSCBundle& operator=(FNI2SNetworkOSCBundle&& RHS);

	virtual OSCPacketType GetType() const override { return OSCPacketType::Bundle;  }

	virtual int32 GetSize() const override;

	void AddElement(const void* InData, const int32 InSize);

	int32 GetElementCount() const;

	const TArray<uint8>& GetElement(const int32 Index) const;

	virtual TArray<uint8> WriteToBuffer() const override;

	virtual void WriteToBuffer(TArray<uint8>& Buffer) const override;

	static TSharedPtr<FNI2SNetworkOSCBundle> CreateFromBuffer(const void* Data, int32 DataLength);

protected:

protected:

	typedef TArray<uint8> ElementData;

	OSCPacketMode			Mode;
	int64					TimeTag;
	TArray<ElementData>		Elements;

	static const ANSICHAR*		BundleHeader;
};