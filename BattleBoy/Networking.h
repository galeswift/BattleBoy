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
		HANDLE mWorkerThread;
	};

	class NetworkInterfaceServer : public NetworkInterface
	{
	public:
		NetworkInterfaceServer() {};
		virtual ~NetworkInterfaceServer();
		virtual void Init();
	};

	class NetworkInterfaceClient : public NetworkInterface
	{
	public:
		NetworkInterfaceClient(std::string ipAddress) : mPendingIP(ipAddress) {};
		virtual ~NetworkInterfaceClient();
		virtual void Init();
	private:
		std::string mPendingIP;
	};
}