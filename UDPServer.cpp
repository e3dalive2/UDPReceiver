#include "UDPServer.hpp"
#include <spdlog/spdlog.h>

#include "Tools.hpp"
#include "source/utf8.h"

UDPServer::UDPServer(asio::io_context &io_context, const std::string &ip, short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port))
{
    spdlog::info("starting UDP server on {}:{}", ip, port);
    start_receive();
}

void UDPServer::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_, [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                handle_receive(bytes_recvd);
                start_receive();
            } else {
                spdlog::error("Error receiving: {}", ec.message());
                start_receive();
            }
        });
}

void UDPServer::handle_receive(std::size_t length)
{
    // Log the sender's IP address and port
    spdlog::info("Received packet from: {}:{}", remote_endpoint_.address().to_string(), remote_endpoint_.port());

    // Log the binary data received

    try {
        std::string text(recv_buffer_.data(), length);
        if (is_ascii_string(recv_buffer_.data(), length)) {
            spdlog::info("Data received (in string): {}", text);
        } else if (utf8::is_valid(text)) {
            spdlog::info("Data received (in utf8): {}", text);
        } else {
            std::string binary_data = to_hex((unsigned char *)recv_buffer_.data(), length);
            spdlog::info("Data received (in binary): {}", binary_data);
        }
    } catch (std::exception &e) {
        spdlog::error("exception {}", e.what());
    }
}
