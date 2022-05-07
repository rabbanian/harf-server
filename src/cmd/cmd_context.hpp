#ifndef HARF_SRC_CMD_CMD_CONTEXT_HPP_
#define HARF_SRC_CMD_CMD_CONTEXT_HPP_

#include <asio/execution_context.hpp>

namespace cmd {

class cmd_context : public asio::execution_context
{
 public:
  class executor;
  typedef executor executor_type;

  cmd_context() {}
  cmd_context(const cmd_context&) = delete;
  cmd_context& operator=(const cmd_context&) = delete;

  executor get_executor() noexcept;

  std::size_t run();
  void stop();
  bool stopped();
  void restart();

 private:
  void post_immediate_completion();
};

class cmd_context::executor
{
 public:
  executor(const executor& from) noexcept : context_(from.context_) {}
  executor(executor&& from) noexcept : context_(from.context_)
  {
    *this = std::move(from);
  }
  ~executor() noexcept {}

  executor& operator=(const executor& from) noexcept { return *this; }
  executor& operator=(executor&& from) noexcept { return *this; }

  bool operator==(const executor& other) const { return true; }
  bool operator!=(const executor& other) const { return !(*this == other); }

  cmd_context& context() const noexcept { return context_; }
  void on_work_started() const noexcept {}
  void on_work_finished() const noexcept {}

  template <typename Function, typename OtherAllocator>
  void dispatch(Function&& f, const OtherAllocator& a) const;

  template <typename Function, typename OtherAllocator>
  void post(Function&& f, const OtherAllocator& a) const;

  template <typename Function, typename OtherAllocator>
  void defer(Function&& f, const OtherAllocator& a) const;

 private:
  friend class cmd_context;

  explicit executor(cmd_context& cntx);

  cmd_context& context_;
};

}  // namespace cmd

#include "impl/cmd_context.hpp"

#endif  // HARF_SRC_CMD_CMD_CONTEXT_HPP_
