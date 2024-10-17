#pragma once

#include <cstddef> //for size_t
#include <string>
#include <vector>

bool IsAsciiString(const char *data, std::size_t length);
bool IsUtf8String(const char *data, std::size_t length);

std::string ToHex(const unsigned char *data, std::size_t length);
long long GetCurrentEpochTime();
std::string ConvertEpochToHumanReadable(long long epoch);