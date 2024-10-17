#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define ENET_IMPLEMENTATION
#include <sockets-cpp/UdpSocket.h>

#include <spdlog/spdlog.h>

class UnicastApp {
public:
	// UDP Multicast
	UnicastApp(const char* remoteAddr, const char* listenAddr, uint16_t localPort, uint16_t port);

	virtual ~UnicastApp() = default;

	void onReceiveData(const char* data, size_t size);

	void sendMsg(const char* data, size_t len);

private:
	sockets::SocketOpt m_socketOpts;
	sockets::UdpSocket<UnicastApp> m_unicast;
};

UnicastApp::UnicastApp(const char* remoteAddr, const char* listenAddr, uint16_t localPort, uint16_t port) : m_socketOpts({ sockets::TX_BUFFER_SIZE, sockets::RX_BUFFER_SIZE, listenAddr }), m_unicast(*this, &m_socketOpts) {
	sockets::SocketRet ret = m_unicast.startUnicast(remoteAddr, localPort, port);
	if (ret.m_success) {
		std::cout << "Listening on UDP " << listenAddr << ":" << localPort << " sending to " << remoteAddr << ":" << port << "\n";
	}
	else {
		std::cout << "Error: " << ret.m_msg << "\n";
		exit(1); // NOLINT
	}
}

void UnicastApp::sendMsg(const char* data, size_t len) {
	auto ret = m_unicast.sendMsg(data, len);
	if (!ret.m_success) {
		std::cout << "Send Error: " << ret.m_msg << "\n";
	}
}

void UnicastApp::onReceiveData(const char* data, size_t size) {
	std::string str(reinterpret_cast<const char*>(data), size);

	std::cout << "Received: " << str << "\n";
}

int main()
{
	const char* addr = "127.0.0.1";
	const char* listenAddr = "127.0.0.1";
	uint16_t port = 27020;
	uint16_t localPort = 27020;

	UnicastApp app(addr, listenAddr, localPort, port);

	while (true) {
		std::string data;
		std::cout << "Data >";
		std::getline(std::cin, data);
		if (data == "quit") {
			break;
		}
		app.sendMsg(data.data(), data.size());
	}

	return 8;
}