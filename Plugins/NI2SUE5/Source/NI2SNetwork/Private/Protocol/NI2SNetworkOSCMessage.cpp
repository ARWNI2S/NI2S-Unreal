// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Protocol/OSC/NI2SNetworkOSCMessage.h"
#include "NI2SNetworkCommon.h"

bool FNI2SNetworkOSCMessage::bIsLegacyConnection = false;

FNI2SNetworkOSCMessage::FNI2SNetworkOSCMessage(OSCPacketMode InMode)
{
	Mode = InMode;
	TagIndex = 0;
	BufferIndex = 0;
}


FNI2SNetworkOSCMessage::FNI2SNetworkOSCMessage(const TCHAR* InAddress)
	: FNI2SNetworkOSCMessage(OSCPacketMode::Write)
{
	SetPath(InAddress);
}

FNI2SNetworkOSCMessage::~FNI2SNetworkOSCMessage()
{
}


FNI2SNetworkOSCMessage::FNI2SNetworkOSCMessage(FNI2SNetworkOSCMessage&& RHS)
{
	*this = MoveTemp(RHS);
}

FNI2SNetworkOSCMessage& FNI2SNetworkOSCMessage::operator=(FNI2SNetworkOSCMessage&& RHS)
{
	Mode = RHS.Mode;
	Address = MoveTemp(RHS.Address);
	TagString = MoveTemp(RHS.TagString);
	TagIndex = RHS.TagIndex;
	Buffer = MoveTemp(RHS.Buffer);
	BufferIndex = RHS.BufferIndex;

	return *this;
}


int FNI2SNetworkOSCMessage::SetPath(const TCHAR* InAddress)
{
	Address = InAddress;
	return 0;
}

void FNI2SNetworkOSCMessage::ResetRead()
{
	check(IsReading());
	TagIndex = 0;
	BufferIndex = 0;
}

int FNI2SNetworkOSCMessage::Read(const TCHAR* InName, FString& Value)
{
	check(IsReading());
	/*if (Mode == OSCPacketMode::Write)
	{
		// string should be null-terminated so allow space for that then
		// terminate
		int32 StartingIndex = BufferIndex;
		int32 StringLen = Value.Len();

		SerializeWrite(TEXT('s'), TCHAR_TO_ANSI(*Value), StringLen + 1);
		Buffer[StartingIndex + StringLen] = 0;
	}
	else*/
	{
		TCHAR CurrentTag = TagString[TagIndex];

		if (CurrentTag != 's')
		{
			UE_LOG(LogNI2SNetwork, Error, TEXT("OSCMessage: Requested tag 's' but next tag was %c"), CurrentTag);
			return 1;
		}

		// get a pointer to the string (which is null-terminated) and read into the value
		ANSICHAR* pString = (ANSICHAR*)(Buffer.GetData() + BufferIndex);
		Value = ANSI_TO_TCHAR(pString);

		// manually adjust our buffer as if we read the string
		BufferIndex += RoundedArgumentSize(Value.Len() + 1);

		TagIndex++;
	}

	return 0;
}

int FNI2SNetworkOSCMessage::Serialize(const TCHAR Code, void* InData, int32 InSize)
{
	if (Mode == OSCPacketMode::Read)
	{
		return ReadTagAndData(Code, InData, InSize);
	}
	else
	{
		return WriteTagAndData(Code, InData, InSize);
	}
}

int FNI2SNetworkOSCMessage::WriteTag(const TCHAR Code)
{
	TagString += Code;
	TagIndex++;
	return 0; 
}

int FNI2SNetworkOSCMessage::WriteTagAndData(const TCHAR Code, const void* InData, int32 InSize)
{
	WriteTag(Code);
	WriteData(InData, InSize);
	return 0;
}

int FNI2SNetworkOSCMessage::WriteData(const void* InData, int32 InSize)
{
	// in OSC every write must be a multiple of 32-bits
	const int32 RoundedSize = RoundedArgumentSize(InSize);

	// allocate space and write.
	Buffer.AddUninitialized(RoundedSize);
	FMemory::Memcpy(Buffer.GetData() + BufferIndex, InData, InSize);
	BufferIndex += RoundedSize;

	return 0;
}

TCHAR FNI2SNetworkOSCMessage::ReadTag(const TCHAR ExpectedTag, bool SuppressWarning /*= false*/)
{
	if (TagIndex == TagString.Len())
	{
		UE_CLOG(!SuppressWarning, LogNI2SNetwork, Error, TEXT("OSCMessage: Cannot read tag %c, no more tags!"), ExpectedTag);
		return 0;
	}

	TCHAR CurrentTag = TagString[TagIndex];

	if (CurrentTag != ExpectedTag)
	{
		UE_CLOG(!SuppressWarning, LogNI2SNetwork, Error, TEXT("OSCMessage: Requested tag %c but next tag was %c"), ExpectedTag, CurrentTag);
		return 0;
	}

	TagIndex++;
	return CurrentTag;
}

int FNI2SNetworkOSCMessage::ReadTagAndData(const TCHAR ExpectedTag, void* InData, int32 InSize)
{
	TCHAR CurrentTag = ReadTag(ExpectedTag);

	if (CurrentTag == 0)
	{
		// readtag will generate errors
		return 1;
	}	
	ReadData(InData, InSize);
	return 0;
}

int FNI2SNetworkOSCMessage::ReadData(void* InData, int32 InSize)
{
	FMemory::Memcpy(InData, Buffer.GetData() + BufferIndex, InSize);
	// in OSC every write must be a multiple of 32-bits
	BufferIndex += RoundedArgumentSize(InSize);
	return 0;
}

/* Read a blob of data from our arguments */
int FNI2SNetworkOSCMessage::Read(const TCHAR* InName, void* OutBlob, int32 MaxBlobSize, int32& OutBlobSize)
{
	check(IsReading());

	bool ReadWasSuccess = false;

	if (ReadTag(TEXT('b')))
	{
		// take a copy here incase the user should use the same var for both args..
		int PassedMaxSize = MaxBlobSize;

		// read the blob size
		ReadData(&OutBlobSize, sizeof(int32));

		if (PassedMaxSize == 0)
		{
			// if MaxBlobSize was 0 then this was just a query so backout the tag and int that was read
			TagIndex--;
			BufferIndex -= sizeof(int32);
			ReadWasSuccess = true; // still success
		}
		else
		{
			if (ensure(OutBlob) && PassedMaxSize >= OutBlobSize)
			{
				ReadData(OutBlob, OutBlobSize);
			}
			else
			{
				// don't read any data, just skip it all so subsequent reads don't access duff data
				BufferIndex += RoundedArgumentSize(OutBlobSize);
				UE_LOG(LogNI2SNetwork, Error, TEXT("OSCMessage: buffer for reading blob was too small. Blob: %d bytes, buffer: %d"), OutBlobSize, MaxBlobSize);
			}
		}

		ReadWasSuccess = (MaxBlobSize >= OutBlobSize || MaxBlobSize == 0);
	}

	return ReadWasSuccess ? 0 : 1;
}

void FNI2SNetworkOSCMessage::GetComponentSizes(int32& OutAddressSize, int32& OutTagSize, int32& OutBufferSize) const
{
	OutAddressSize = RoundedArgumentSize(GetPath().Len()+1);

	const FString FinalTagString = FString::Printf(TEXT(",%s"), *GetTags());

	OutTagSize = RoundedArgumentSize(FinalTagString.Len()+1);		// we don't store the , internally

	OutBufferSize = BufferIndex;
}

int32 FNI2SNetworkOSCMessage::GetSize() const
{
	int32 AddressSize, TagSize, BufferSize;
	GetComponentSizes(AddressSize, TagSize, BufferSize);

	return AddressSize + TagSize + BufferSize;
}

TArray<uint8> FNI2SNetworkOSCMessage::WriteToBuffer() const
{
	TArray<uint8> OutBuffer;
	WriteToBuffer(OutBuffer);
	return OutBuffer;
}

void FNI2SNetworkOSCMessage::WriteToBuffer(TArray<uint8>& OutBuffer) const
{
	int32 AddressSize(0);
	int32 TagSize(0);
	int32 BufferSize(0);
	GetComponentSizes(AddressSize, TagSize, BufferSize);

	const int32 kBufferOffset = OutBuffer.Num();
	const int32 kRequiredSize = AddressSize + TagSize + BufferSize;

	OutBuffer.AddUninitialized(kRequiredSize);

	ANSICHAR* pOutBuffer = (ANSICHAR*)OutBuffer.GetData() + kBufferOffset;
	
	const int32 kAddressLength = GetPath().Len()+1;
	const FString FinalTagString = FString::Printf(TEXT(",%s"), *GetTags());
	const int32 kTagLength = FinalTagString.Len() + 1;

	FCStringAnsi::Strncpy(pOutBuffer, TCHAR_TO_ANSI(*GetPath()), kAddressLength);
	pOutBuffer[kAddressLength] = 0;
	pOutBuffer += AddressSize;

	FCStringAnsi::Strcpy(pOutBuffer, kTagLength, TCHAR_TO_ANSI(*FinalTagString));
	pOutBuffer[kTagLength] = 0;
	pOutBuffer += TagSize;

	FMemory::Memcpy(pOutBuffer, Buffer.GetData(), BufferIndex);
}

TSharedPtr<FNI2SNetworkOSCMessage> FNI2SNetworkOSCMessage::CreateFromBuffer(const void * Data, int32 DataLength)
{
	const uint8* pParsedData = (const uint8*)Data;

	TSharedPtr<FNI2SNetworkOSCMessage> NewMessage = MakeShareable(new FNI2SNetworkOSCMessage(OSCPacketMode::Read));

	// first argument is the address as a null-terminated char*
	NewMessage->Address = ANSI_TO_TCHAR((ANSICHAR*)pParsedData);
	pParsedData += RoundedArgumentSize(NewMessage->Address.Len() + 1);

	// second argument is the address as a null-terminated char*
	FString TagString = ANSI_TO_TCHAR((ANSICHAR*)pParsedData);

	// remove the leading ,
	NewMessage->TagString = TagString.RightChop(TagString.Find(TEXT(","), ESearchCase::CaseSensitive)+1);

	pParsedData += RoundedArgumentSize(TagString.Len() + 1);

	// now pData points at the arguments.
	const int kArgLength = DataLength - (pParsedData - (const uint8*)Data);
	
	NewMessage->Buffer.AddUninitialized(kArgLength);
	FMemory::Memcpy(NewMessage->Buffer.GetData(), pParsedData, kArgLength);

	return NewMessage;
}

/*

FNI2SNetworkOSCMessage& operator << (FNI2SNetworkOSCMessage& Msg, int32& Value)
{
	Msg.Serialize(Value);
	return Msg;
}

FNI2SNetworkOSCMessage& operator << (FNI2SNetworkOSCMessage& Msg, float& Value)
{
	Msg.Serialize(Value);
	return Msg;
}

FNI2SNetworkOSCMessage& operator << (FNI2SNetworkOSCMessage& Msg, bool& Value)
{
	if (Msg.IsWriting())
	{
		int32 IntValue = Value ? 1 : 0;
		Msg.Serialize(IntValue);
	}
	else
	{
		int32 IntValue(0);
		Msg.Serialize(IntValue);
		Value = IntValue == 0 ? false : true;
	}
	return Msg;
}

FNI2SNetworkOSCMessage& operator << (FNI2SNetworkOSCMessage& Msg, TCHAR& Value)
{
	if (Msg.IsWriting())
	{
		int32 IntValue = Value;
		Msg.Serialize(IntValue);
	}
	else
	{
		int32 IntValue(0);
		Msg.Serialize(IntValue);
		Value = (TCHAR)IntValue;
	}
	return Msg;
}

FNI2SNetworkOSCMessage& operator << (FNI2SNetworkOSCMessage& Msg, FString& Value)
{
	Msg.Serialize(Value);
	return Msg;
}

*/
