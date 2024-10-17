#include "Record.hpp"
#include "source/utf8.h"
#include <regex>
#include <spdlog/spdlog.h>
#include "Tools.hpp"

Record::Record(const char *data, size_t len, const std::string &source)
    : blob_{data, data + len}
    , source_{source}
{
}

void Record::process()
{
    std::string text(blob_.data(), blob_.size());

    if (is_ascii_string(blob_.data(), blob_.size())) {
        spdlog::info("Data received (in string): {}", text);
    } else if (utf8::is_valid(text)) {
        spdlog::info("Data received (in utf8): {}", text);
    } else {
        std::string binary_data = to_hex((unsigned char *)blob_.data(), blob_.size());
        spdlog::info("Data received (in binary): {}", binary_data);
    }
}
