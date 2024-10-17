#pragma once

#include <asio/include/asio.hpp>

using asio::ip::udp;

constexpr auto kMaxUDPBufferSize = 65507 - 8; // max size of UDP datagram minus the header
class UDPServer
{
public:
    UDPServer(asio::io_context &io_context, const std::string &ip, short port);

private:
    void start_receive();
    void handle_receive(std::size_t length);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, kMaxUDPBufferSize> recv_buffer_;
};