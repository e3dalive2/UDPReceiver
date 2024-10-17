
#include <spdlog/spdlog.h>

#include "Tools.hpp"
#include "UDPServer.hpp"
#include "Logger.hpp"

int main()
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    uint16_t port = 27020;
    std::string bindIP = "127.0.0.1";
    std::string filterString = ".*";
    std::string recordPath = "records.txt";

    try {
        asio::io_context io_context;

        auto logger = std::make_shared<Logger>(recordPath);

        UDPServer server(io_context, bindIP, port); // Listening on port 12345
        server.setFilter(filterString);
        server.setLogger(logger);

        io_context.run(); // Start the ASIO event loop
    } catch (std::exception &e) {
        spdlog::error("Exception: {}", e.what());
    }
    return 8;
}