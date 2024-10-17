#include "Logger.hpp"


Logger::Logger(const std::string &targetFile)
    : ready(false)
    , worker_thread(&Logger::run, this)
{
    
}

void Logger::addData(char *data, std::size_t len, const std::string &source)
{
    Record record(data, len, source);
    q_.enqueue(std::move(record));
}

void Logger::run()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return ready; }); // Wait until ready is true

}