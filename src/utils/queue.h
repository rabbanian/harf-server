#ifndef HARF_QUEUE_H
#define HARF_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace utils
{

template <typename T>
class queue
{
  private:
    std::queue<T> buffer;
    std::mutex mx;
    std::condition_variable cv;

  public:
    queue() = default;
    queue(const queue<T> &rhs) = delete;

    T pop()
    {
        std::lock_guard lock(mx);
        auto el = std::move(buffer.front());
        buffer.pop();
        return el;
    }

    void push(const T& el)
    {
        std::lock_guard lock(mx);
        buffer.emplace(std::move(el));
    }

    void push(T&& el)
    {
        std::lock_guard lock(mx);
        buffer.emplace(std::move(el));
    }

    auto size()
    {
        return buffer.size();
    }
};

} // namespace utils

#endif // HARF_QUEUE_H
