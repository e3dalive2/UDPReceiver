#include "UDPServer.hpp"
#include <spdlog/spdlog.h>

UDPServer::UDPServer(asio::io_context& io_context, const std::string& ip,short port)
	: socket_(io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port))
{
	start_receive();
}

void UDPServer::start_receive()
{
	socket_.async_receive_from(
		asio::buffer(recv_buffer_), remote_endpoint_,
		[this](std::error_code ec, std::size_t bytes_recvd) {
			if (!ec && bytes_recvd > 0) {
				handle_receive(bytes_recvd);
			}
			else {
				start_receive();
			}
		});
}

void UDPServer::handle_receive(std::size_t length)
{
	// Log the sender's IP address and port
	spdlog::info("Received packet from: {}:{}", remote_endpoint_.address().to_string(), remote_endpoint_.port());

	// Log the binary data received
	spdlog::info("Data received (in binary):");
	std::string binary_data;
	for (std::size_t i = 0; i < length; ++i) {
		binary_data += fmt::format("{:02X} ", static_cast<unsigned char>(recv_buffer_[i]));
	}
	spdlog::info(binary_data);

	// Echo the received data back to the sender
	socket_.async_send_to(
		asio::buffer(recv_buffer_, length), remote_endpoint_,
		[this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
			start_receive();
		});
}

