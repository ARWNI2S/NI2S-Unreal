// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SNetwork/Transport/INI2SNetworkTransport.h"
#include "NI2SNetwork/Utils/NI2SNetworkThreadedConnection.h"
#include "NI2SNetworkCommon.h"
#include "HAL/PlatformProcess.h"
#include "Misc/AutomationTest.h"

PRAGMA_DISABLE_OPTIMIZATION

#if WITH_DEV_AUTOMATION_TESTS && 0

class FNI2SNetworkTestTransport : public FAutomationTestBase
{
	enum
	{
		DefaultPort = 2020
	};
public:

	FNI2SNetworkTestTransport(const FString& InName, const bool bInComplexTask)
		: FAutomationTestBase(InName, bInComplexTask) {}

	void CreateListener()
	{
		if (INI2SNetworkTransport* BC = INI2SNetworkTransport::Get())
		{
			ListenerConnection = BC->CreateConnection(INI2SNetworkTransport::TCP);
		}
	}

	void CreateConnection1()
	{
		if (INI2SNetworkTransport* BC = INI2SNetworkTransport::Get())
		{
			ClientConnection = BC->CreateConnection(INI2SNetworkTransport::TCP);
		}
	}
	
	bool ConnectListenerAndConnection()
	{
		FThreadSafeBool WaitingForConnect;
		FThreadSafeBool ServerIsConnected;

		ListenerConnection->Listen(DefaultPort);
		ClientConnection->Connect(*FString::Printf(TEXT("127.0.0.1:%d"), (int)DefaultPort));

		bool ClientConnected = false;
		bool AcceptConnected = false;

		FNI2SNetworkThreadedListener ThreadedConnection;

		ThreadedConnection.Start(ListenerConnection.ToSharedRef(), 
			FNI2SNetworkListenerDelegate::CreateLambda([this, &AcceptConnected](TSharedPtr<INI2SNetworkSocketConnection> NewConnection)
		{
			AcceptConnected = true;
			AcceptedConnection = NewConnection;
			return true;
		}));

		do 
		{
			if (ClientConnected == false)
			{
				ClientConnection->WaitForConnection(0, [&ClientConnected](TSharedPtr<INI2SNetworkSocketConnection> NewConnection) {
					ClientConnected = true;
					return true;
				});
			}

			FPlatformProcess::SleepNoStats(0.5);

		} while (!ClientConnected || !AcceptConnected);

		return AcceptedConnection.IsValid();
	}

	TSharedPtr<INI2SNetworkSocketConnection>	ListenerConnection;
	TSharedPtr<INI2SNetworkSocketConnection>	ClientConnection;
	TSharedPtr<INI2SNetworkSocketConnection>	AcceptedConnection;

};

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FNI2SNetworkTestCreate, FNI2SNetworkTestTransport, "NI2SNetwork.TestTransport", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FNI2SNetworkTestCreate::RunTest(const FString& Parameters)
{
	CreateListener();
	CreateConnection1();

	return ListenerConnection.IsValid() && ClientConnection.IsValid();
}


IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FNI2SNetworkTestConnect, FNI2SNetworkTestTransport, "Project.NI2SNetwork.TestConnect", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FNI2SNetworkTestConnect::RunTest(const FString& Parameters)
{
	CreateListener();
	CreateConnection1();

	ConnectListenerAndConnection();

	return ListenerConnection.IsValid() && ClientConnection.IsValid() && AcceptedConnection.IsValid();	
}

IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FNI2SNetworkTestSendReceive, FNI2SNetworkTestTransport, "Project.NI2SNetwork.TestConnect", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FNI2SNetworkTestSendReceive::RunTest(const FString& Parameters)
{
	CreateListener();
	CreateConnection1();
	ConnectListenerAndConnection();

	FString Msg = TEXT("Hello!");
	TCHAR MsgReceived[256] = { 0 };


	int32 MsgSize = Msg.Len() * sizeof(TCHAR);

	int32 Sent = ClientConnection->SendData(*Msg, MsgSize);

	check(Sent == MsgSize);

	int32 Received = AcceptedConnection->ReceiveData(MsgReceived, 256);

	check(Received == Sent);

	check(Msg == MsgReceived);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

PRAGMA_ENABLE_OPTIMIZATION
