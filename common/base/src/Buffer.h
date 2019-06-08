#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_
#include <cstring>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer
{
public:
    FixedBuffer()
        :cur_(data_)
    { }

    ~FixedBuffer() {  }

    FixedBuffer(const FixedBuffer& other) = delete;
    FixedBuffer& operator=(const FixedBuffer& other) = delete;

    void Append(const char* buf, size_t len)
    {
        if (Avail() > len)
        {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char* Data() const { return cur_; }
    int Length() const { return static_cast<int>((cur_) - data_); }

    char* Current() { return cur_; }
    int Avail() const { return static_cast<int>(End() - cur_); }

    void Reset() { cur_ = data_; }
    void BZero() { memset(data_, 0, sizeof(data_)); }

private:
    const char* End() const { return data_ + sizeof(data_); }
    char data_[SIZE];
    char* cur_;
};

#endif // BASE_BUFFER_H_
