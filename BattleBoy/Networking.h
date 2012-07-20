#pragma once

namespace Networking
{
	enum ENetworkingRole
	{
		ROLE_None,
		ROLE_Authority,
		ROLE_SimulatedProxy,
	};

	class NetworkInterface
	{
	public:
		virtual void Init() {};
	};

	class NetworkInterfaceServer : public NetworkInterface
	{
	public:
		NetworkInterfaceServer() {};
		virtual ~NetworkInterfaceServer();
		virtual void Init();
		HANDLE mWorkerThread;
	};

	class NetworkInterfaceClient : public NetworkInterface
	{
	public:
		NetworkInterfaceClient() {};
		virtual ~NetworkInterfaceClient(){};
		virtual void Init() {};
	};
}