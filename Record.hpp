#pragma once

#include <vector>
#include <string>

class Record
{
public:
    Record(const char *data, size_t len,const std::string &source);

    std::vector<char> blob_;
    std::string source_;

    void process();
};