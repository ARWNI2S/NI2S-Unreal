// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Transport/INI2SNetworkTransport.h"
#include "NI2SNetworkCommon.h"
#include "Transport/NI2SNetworkConnection.h"

const TCHAR* NI2SNetworkTransport_TCP = TEXT("NI2SNetworkTCP");

DEFINE_LOG_CATEGORY(LogNI2SNetwork);

const int32 INI2SNetworkTransport::TCP=1;

class FNI2SNetworkTransport : public INI2SNetworkTransport
{
public:


	virtual TSharedPtr<INI2SNetworkSocketConnection> CreateConnection(const int32 Type) override
	{
		check(Type == INI2SNetworkTransport::TCP);
		TSharedPtr<FNI2SNetworkConnection> Connection = MakeShareable(new FNI2SNetworkConnection());
		return Connection;	
	}
};

IMPLEMENT_MODULE(FNI2SNetworkTransport, NI2SNetwork)

