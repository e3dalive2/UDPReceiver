#pragma once

#include <cstddef> //for size_t
#include <string>
#include <vector>

bool is_ascii_string(const char *data, std::size_t length);
bool is_utf8_string(const char *data, std::size_t length);

std::string to_hex(const unsigned char *data, std::size_t length);
long long getCurrentEpochTime();
std::string convertEpochToHumanReadable(long long epoch);