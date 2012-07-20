#pragma once

namespace Networking
{
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
}