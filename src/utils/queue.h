#ifndef HARF_SRC_UTILS_QUEUE_H_
#define HARF_SRC_UTILS_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

namespace utils {

template <typename T>
class Queue
{
 public:
  Queue() = default;
  Queue(const Queue<T> &rhs) = delete;

  T Pop()
  {
    std::lock_guard lock(mx_);
    auto el = std::move(buffer_.front());
    buffer_.pop();
    return el;
  }

  void Push(const T &el)
  {
    std::lock_guard lock(mx_);
    buffer_.emplace(std::move(el));
    cv_.notify_all();
  }

  void Push(T &&el)
  {
    std::lock_guard lock(mx_);
    buffer_.emplace(std::move(el));
    cv_.notify_all();
  }

  bool Wait()
  {
    while (Empty()) {
      std::unique_lock lock(wmx_);
      cv_.wait(lock);
    }
    return true;
  }

  bool Empty()
  {
    std::lock_guard lock(mx_);
    return buffer_.empty();
  }

  auto Size()
  {
    std::lock_guard lock(mx_);
    return buffer_.size();
  }

 private:
  std::queue<T> buffer_;
  std::mutex mx_;
  std::condition_variable cv_;
  std::mutex wmx_;
};

}  // namespace utils

#endif  // HARF_SRC_UTILS_QUEUE_H_
