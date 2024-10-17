#pragma once

#include <asio/include/asio.hpp>
#include <memory>

#include "Logger.hpp"

using audp=asio::ip::udp;

constexpr auto kMaxUDPBufferSize = 65507 - 8; // max size of UDP datagram minus the header

using shLogger=std::shared_ptr<Logger>;
class UDPServer
{
public:
    UDPServer(asio::io_context &io_context, const std::string &ip, short port);

    inline void setFilter(std::string &filter) { filter_ = filter; }
    inline void setLogger(shLogger logger) { logger_ = logger; }

private:
    void start_receive();
    void handle_receive(std::size_t length);

    audp::socket socket_;
    audp::endpoint remote_endpoint_;
    std::array<char, kMaxUDPBufferSize> recv_buffer_;
    std::string filter_;
    shLogger logger_;
};