#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define ENET_IMPLEMENTATION


#include <spdlog/spdlog.h>

#include <asio/include/asio.hpp>

using asio::ip::udp;

class UDPServer {
public:
	UDPServer(asio::io_context& io_context, short port)
		: socket_(io_context, udp::endpoint(udp::v4(), port)) {
		start_receive();
	}

private:
	void start_receive() {
		// Prepare buffer for receiving data
		socket_.async_receive_from(
			asio::buffer(recv_buffer_), remote_endpoint_,
			[this](std::error_code ec, std::size_t bytes_recvd) {
				if (!ec && bytes_recvd > 0) {
					handle_receive(bytes_recvd);
				}
				else {
					start_receive(); // Continue receiving on error or empty
				}
			});
	}

	void handle_receive(std::size_t length) {
		// Print the sender's IP address and port
		std::cout << "Received packet from: "
			<< remote_endpoint_.address().to_string() << ":"
			<< remote_endpoint_.port() << "\n";

		// Print binary data received
		std::cout << "Data received (in binary): ";
		for (std::size_t i = 0; i < length; ++i) {
			printf("%02X ", static_cast<unsigned char>(recv_buffer_[i]));
		}
		std::cout << "\n";

		// Echo the received data back to the sender
		socket_.async_send_to(
			asio::buffer(recv_buffer_, length), remote_endpoint_,
			[this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
				start_receive();  // Start waiting for the next message
			});
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	std::array<char, 1024> recv_buffer_; // Buffer for receiving data
};

int main()
{
	uint16_t port = 27020;


	try {
		asio::io_context io_context;

		UDPServer server(io_context, port); // Listening on port 12345

		io_context.run(); // Start the ASIO event loop
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 8;
}