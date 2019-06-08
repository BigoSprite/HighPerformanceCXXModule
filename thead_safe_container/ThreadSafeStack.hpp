//
// Created by fm on 2019/5/12.
//

#ifndef THREAD_SAFE_CONTAINER_STACK_HPP_
#define THREAD_SAFE_CONTAINER_STACK_HPP_

#include <mutex>
#include <stack>

template<typename T>
class ThreadSafeStack
{
public:
    ThreadSafeStack();
    ~ThreadSafeStack();
    ThreadSafeStack(const ThreadSafeStack& other) = delete;
    ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

    int Push(T val);
    int Pop(T& val);
    bool Empty() const;

private:
    std::stack<T> stack_;
    mutable std::mutex mtx_;
};

template<typename T>
ThreadSafeStack<T>::ThreadSafeStack(){
}

template<typename T>
ThreadSafeStack<T>::~ThreadSafeStack() {

}

template<typename T>
int ThreadSafeStack<T>::Push(T val) {
    std::lock_guard<std::mutex> mtx(mtx_);
    stack_.push(std::move(val));
    return 0;
}

template<typename T>
int ThreadSafeStack<T>::Pop(T& val) {
    std::lock_guard<std::mutex> mtx(mtx_);
    if (stack_.empty()) {
        return -1;
    }

    val = std::move(stack_.top());
    stack_.pop();
    return 0;
}

template<typename T>
bool ThreadSafeStack<T>::Empty() const {
    std::lock_guard<std::mutex> mtx(mtx_);
    return stack_.empty();
}

#endif // THREAD_SAFE_CONTAINER_STACK_HPP_
