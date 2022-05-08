#ifndef HARF_SRC_AMC_AMC_CONTEXT_HPP_
#define HARF_SRC_AMC_AMC_CONTEXT_HPP_

#include <asio/execution_context.hpp>

namespace amc {

class amc_context : public asio::execution_context
{
 public:
  class executor;
  typedef executor executor_type;

  amc_context() {}
  amc_context(const amc_context&) = delete;
  amc_context& operator=(const amc_context&) = delete;

  executor get_executor() noexcept;

  std::size_t run();
  void stop();
  bool stopped();
  void restart();

 private:
  void post_immediate_completion();
};

class amc_context::executor
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

  amc_context& context() const noexcept { return context_; }
  void on_work_started() const noexcept {}
  void on_work_finished() const noexcept {}

  template <typename Function, typename OtherAllocator>
  void dispatch(Function&& f, const OtherAllocator& a) const;

  template <typename Function, typename OtherAllocator>
  void post(Function&& f, const OtherAllocator& a) const;

  template <typename Function, typename OtherAllocator>
  void defer(Function&& f, const OtherAllocator& a) const;

 private:
  friend class amc_context;

  explicit executor(amc_context& cntx);

  amc_context& context_;
};

}  // namespace amc

#include "impl/amc_context.hpp"

#endif  // HARF_SRC_AMC_AMC_CONTEXT_HPP_
