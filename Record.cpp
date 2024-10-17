#include "Record.hpp"
#include "Tools.hpp"
#include "source/utf8.h"
#include <regex>
#include <spdlog/spdlog.h>

Record::Record(const char *data, size_t len, const std::string &source, long long stamp)
    : blob_{data, data + len}
    , source_{source}
    , stamp_{stamp}
{
}

Record::Record()
{
}

void Record::process(std::ofstream &f)
{
    std::string text(blob_.data(), blob_.size());

    auto date = convertEpochToHumanReadable(stamp_);

    f << source_ << " " << date << " ";
    if (is_ascii_string(blob_.data(), blob_.size())) {
        spdlog::info("Data received (in string): {}", text);
        f << "ascii: " << text;
    } else if (utf8::is_valid(text)) {
        spdlog::info("Data received (in utf8): {}", text);
        f << "utf8: " << text;
    } else {
        std::string binary_data = to_hex((unsigned char *)blob_.data(), blob_.size());
        spdlog::info("Data received (in binary): {}", binary_data);
        f << "bin: " << binary_data;
    }
    f << std::endl;
}
