#pragma once

#include <vector>
#include <string>
#include <fstream>

class Record
{
public:
    Record(const char *data, size_t len,const std::string &source,long long stamp);
    Record();

    std::vector<char> blob_;
    std::string source_;
    long long stamp_;

    void process(std::ofstream &f);
};