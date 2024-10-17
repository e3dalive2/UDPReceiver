#include "Tools.hpp"

#include <codecvt>
#include <spdlog/fmt/chrono.h>
#include <spdlog/spdlog.h>

bool IsAsciiString(const char *data, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++i) {
        if (data[i] < 32 || data[i] > 126) // ASCII printable range
        {
            return false;
        }
    }
    return true;
}

bool IsUtf8String(const char *data, std::size_t length)
{
    try {
        std::string str(data, length);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        conv.from_bytes(str); // Try converting to a wide string
        return true;          // Valid UTF-8
    } catch (const std::range_error &) {
        return false; // Invalid UTF-8
    }
}

std::string ToHex(const unsigned char *data, std::size_t length)
{
    std::string hex_str;
    for (std::size_t i = 0; i < length; i++) {
        hex_str += fmt::format("{:02x}", data[i]); // Format each byte as hex (2 digits, lowercase)
    }
    return hex_str;
}

long long GetCurrentEpochTime()
{
    // Get current time point
    auto now = std::chrono::system_clock::now();

    // Convert to epoch time (seconds)
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    return epoch_time;
}

std::string ConvertEpochToHumanReadable(long long epoch)
{
    // Convert epoch time to time_t
    std::time_t time = static_cast<std::time_t>(epoch);

    // Convert to local time
    std::tm local_time = *std::localtime(&time);

    // Format the time using fmt library
    return fmt::format("{:%Y-%m-%d %H:%M:%S}", local_time);
}