#include "Logger.hpp"
#include "Tools.hpp"
#include <spdlog/spdlog.h>

Logger::Logger(const std::string &targetFile)
    : ready_(false)
    , worker_thread(&Logger::run, this)
{
    f.open(targetFile);
    if (!f.is_open()) {
        spdlog::error("failure when opening file for writing {}", targetFile);
    }
}

Logger::~Logger()
{
    // Signal that logging is done
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ready_ = true; // Set ready to true to allow the worker thread to proceed
    }

    cv_.notify_all(); // Notify all consumers to exit

    if (worker_thread.joinable()) {
        worker_thread.join(); // Wait for the worker thread to finish
    }
}

void Logger::addData(char *data, std::size_t len, const std::string &source)
{
    Record record(data, len, source, GetCurrentEpochTime());
    q_.enqueue(std::move(record));
    cv_.notify_one(); // Notify one waiting consumer
}

void Logger::run()
{
    // Set ready to true to start processing
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ready_ = true;
    }

    while (true) {
        Record record;

        // Attempt to dequeue a record
        if (q_.try_dequeue(record)) {
            // Process the record (e.g., write to file)
            // You can implement your logging logic here
            if (f.is_open()) {
                record.process(f);
            }
        } else {
            // If there are no records in the queue, wait for a notification
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [&]() { return !ready_ || q_.size_approx() > 0; }); // Wait for a new record or exit signal

            // If we're not ready, break the loop to exit
            if (!ready_) {
                break;
            }
        }
    }
}