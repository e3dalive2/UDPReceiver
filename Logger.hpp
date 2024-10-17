#pragma once

#include "Record.hpp"
#include <memory>
#include <string>
#include <mutex>

#include <concurrentqueue.h>
namespace mc = moodycamel;

class Logger
{
public:
    Logger(const std::string &targetFile);

    // thread-safe
    void addData(char *data, std::size_t len,const std::string &source);

	void run();

private:
    std::thread worker_thread;
    std::condition_variable cv_;
    std::mutex mutex_;
    bool ready;

    mc::ConcurrentQueue<Record> q_;
};