#include "AsyncLogging.h"

AsyncLogging::AsyncLogging(const std::string& basename, off_t roll_size, int flush_interval)
    : basename_(basename),
    roll_size_(roll_size),
    flush_interval_(flush_interval),
    current_buffer_(new Buffer),
    next_buffer_(new Buffer)
{
}

void AsyncLogging::Append(const char* log_line, int len)
{
    std::lock_guard<std::mutex> _(mutex_);
    if (current_buffer_->Avail() > len)
    {
        current_buffer_->Append(log_line, len);
    }
    else
    {
        buffers_.push_back(std::move(current_buffer_));

        if (next_buffer_)
        {
            current_buffer_ = std::move(next_buffer_);
        }
        else
        {
            current_buffer_.reset(new Buffer);// Rarely happens
        }

        current_buffer_->Append(log_line, len);
        cond_.notify_one();
    }
}

void AsyncLogging::Process()
{
    assert(running_);

    LogFile output(basename_, roll_size_, false);
    BufferPtr buf1(new Buffer);
    BufferPtr buf2(new Buffer);
    buf1->BZero();
    buf2->BZero();
    BufferVector buffers_to_write;
    buffers_to_write.reserve(16);

    while (running_)
    {
        assert(buf1 && buf1->Length() == 0);
        assert(buf1 && buf2->Length() == 0);

        {
            std::unique_lock<std::mutex> lk(mutex_);
            if (buffers_.empty())
            {
                //cond_.wait_for(lk, 100);
            }

            buffers_.push_back(std::move(current_buffer_));
            current_buffer_ = std::move(buf1);
            buffers_to_write.swap(buffers_);
            if (!next_buffer_)
            {
                next_buffer_ = std::move(buf2);
            }
        }

        assert(!buffers_to_write.empty());

        for (const auto& buffer : buffers_to_write)
        {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            output.Append(buffer->Data(), buffer->Length());
        }

        if (buffers_to_write.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffers_to_write.resize(2);
        }

        if (!buf1)
        {
            assert(!buffers_to_write.empty());
            buf1 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            buf1->Reset();
        }

        if (!buf2)
        {
            assert(!buffers_to_write.empty());
            buf2 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            buf2->Reset();
        }

        buffers_to_write.clear();
        output.Flush();
    }
    output.Flush();
}
