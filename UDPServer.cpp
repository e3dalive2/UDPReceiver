#include "UDPServer.hpp"
#include <spdlog/spdlog.h>

#include "source/utf8.h"
#include "Tools.hpp"
#include <regex>


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
    std::string source = fmt::format("{}:{}", remote_endpoint_.address().to_string(), remote_endpoint_.port());
    spdlog::info("Received packet from: {}", source);

    // Log the binary data received

    try {
        std::string text(recv_buffer_.data(), length);

        if (utf8::is_valid(text))//filter for text/utf8 only
        {
            std::regex pattern(filter_);
            if (!std::regex_search(text, pattern)) {
                spdlog::warn("filtered");
                return; // filter
            }
        }
        

        if (logger_)
        {
            logger_->addData(recv_buffer_.data(), length, source);
        }
    } catch (std::exception &e) {
        spdlog::error("exception {}", e.what());
    }
}
