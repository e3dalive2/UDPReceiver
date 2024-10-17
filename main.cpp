
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#include <spdlog/spdlog.h>

#include "UDPServer.hpp"
#include "Tools.hpp"

int main()
{
	uint16_t port = 27020;
	std::string bindIP = "127.0.0.1";

	try {
		asio::io_context io_context;

		UDPServer server(io_context, bindIP, port); // Listening on port 12345

		io_context.run(); // Start the ASIO event loop
	}
	catch (std::exception& e) {
		spdlog::error("Exception: {}", e.what());
	}

	return 8;
}