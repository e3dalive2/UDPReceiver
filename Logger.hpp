#pragma once
#include "Record.hpp"
#include <condition_variable>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

#include <concurrentqueue.h>
namespace mc = moodycamel;

class Logger
{
public:
    Logger(const std::string &targetFile);
    ~Logger();
    // thread-safe
    void addData(char *data, std::size_t len, const std::string &source);
    void run();

private:
    std::thread worker_thread;
    std::condition_variable cv_;
    std::mutex mutex_;
    bool ready_;

    mc::ConcurrentQueue<Record> q_;
    std::ofstream f;
};