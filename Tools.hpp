#pragma once

#include <cstddef> //for size_t

bool is_ascii_string(const char* data, std::size_t length);
bool is_utf8_string(const char* data, std::size_t length);