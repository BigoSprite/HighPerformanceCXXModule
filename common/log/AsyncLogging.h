#ifndef LOG_ASYNCLOGGING_H_
#define LOG_ASYNCLOGGING_H_

#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "../base/src/Buffer.h"
#include <vector>
#include <memory>

class AsyncLogging
{
public:
    AsyncLogging(const std::string& basename,
        off_t roll_size,
        int flush_interval = 3);

    ~AsyncLogging()
    {
        if (running_)
        {
            Stop();
        }
    }

    AsyncLogging(const AsyncLogging& other) = delete;
    AsyncLogging& operator=(const AsyncLogging& other) = delete;

    void Append(const char* log_line, int len);

    void Start()
    {
        running_ = true;

        // Æô¶¯Ïß³Ì
        std::thread th(&AsyncLogging::Process, this);
        thread_ = std::move(th);
    }

    void Stop()
    {
        running_ = false;
        cond_.notify_one();
        thread_.join();
    }

private:

    void Process();

    using Buffer       = FixedBuffer<kLargeBuffer>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr    = BufferVector::value_type;

    std::atomic<bool> running_;
    const std::string basename_;
    const off_t roll_size_;
    const int flush_interval_;

    std::mutex mutex_;
    std::condition_variable cond_;
    std::thread thread_;

    BufferPtr    current_buffer_;
    BufferPtr    next_buffer_;
    BufferVector buffers_;

    

};


#endif // LOG_ASYNCLOGGING_H_
