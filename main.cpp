#include <regex>

#include <spdlog/spdlog.h>

#include "Logger.hpp"
#include "Tools.hpp"
#include "UDPServer.hpp"

void print_help()
{
    spdlog::info("Usage: UDPReciever [options]");
    spdlog::info("Options:");
    spdlog::info("  --port <port>              Port number (default: 27020)");
    spdlog::info("  --bindIP <ip_address>      IP address to bind (default: 127.0.0.1)");
    spdlog::info("  --filter <regex>           Filter string (default: '.*')");
    spdlog::info("  --recordPath <path>        Path to record file (default: 'records.txt')");
    spdlog::info("  --help                     Display this help message");
}

int main(int argc, char *argv[])
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    uint16_t port = 27020;
    std::string bindIP = "127.0.0.1";
    std::string filterString = ".*";
    std::string recordPath = "records.txt";

    try
    {
        // Command line argument parsing
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--help") {
                print_help();
                return 0;
            } else if (arg == "--port" && (i + 1 < argc)) {
                auto uPort = static_cast<uint16_t>(std::atoi(argv[++i]));
                if (uPort == 0) {
                    spdlog::error("Invalid port number provided. Using default: {}", uPort);
                    port = 27020;
                }
                else
                {
                    port = uPort;
                }
            } else if (arg == "--bindIP" && (i + 1 < argc)) {
                bindIP = argv[++i];
            } else if (arg == "--filter" && (i + 1 < argc)) {
                filterString = argv[++i];
                try {
                    std::regex rgx(filterString);
                } catch (const std::regex_error &e) {
                    spdlog::error("Invalid regex provided: {}", e.what());
                    return 1;
                }
            } else if (arg == "--recordPath" && (i + 1 < argc)) {
                recordPath = argv[++i];
            } else {
                spdlog::warn("Unknown argument: {}", arg);
            }
        }
    }
    catch (std::exception e)
    {
        spdlog::error("parsing params failed with {}", e.what());
    }

    try {
        asio::io_context io_context;

        auto logger = std::make_shared<Logger>(recordPath);

        UDPServer server(io_context, bindIP, port); // Listening on port 12345
        spdlog::info("filter {}", filterString);
        spdlog::info("output {}", recordPath);

        server.setFilter(filterString);
        server.setLogger(logger);

        io_context.run(); // Start the ASIO event loop
    } catch (std::exception &e) {
        spdlog::error("Exception: {}", e.what());
    }
    return 8;
}