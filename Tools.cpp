#include "Tools.hpp"

#include <codecvt>

bool is_ascii_string(const char* data, std::size_t length)
{
	for (std::size_t i = 0; i < length; ++i)
	{
		if (data[i] < 32 || data[i] > 126)  // ASCII printable range
		{
			return false;
		}
	}
	return true;
}

bool is_utf8_string(const char* data, std::size_t length)
{
	try
	{
		std::string str(data, length);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		conv.from_bytes(str);  // Try converting to a wide string
		return true;           // Valid UTF-8
	}
	catch (const std::range_error&)
	{
		return false;          // Invalid UTF-8
	}
}
