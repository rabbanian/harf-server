#ifndef HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_
#define HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_

#include <iostream>

namespace amc {

inline amc_context::amc_context() : stopped_(false) { ++outstanding_work_; }

inline amc_context::executor_type amc_context::get_executor() noexcept
{
  return executor_type(*this);
}

inline std::size_t amc_context::run()
{
  std::size_t n = 0;

  while (!stopped_) {
    if (!(call_operations_.empty() || accept_operations_.empty())) {
      auto cop = std::move(call_operations_.front());
      auto aop = std::move(accept_operations_.front());
      call_operations_.pop();
      accept_operations_.pop();
      aop.connection_ = std::move(cop.connection_);
      aop.packet_ = std::move(cop.packet_);
      aop();
      n++;
      --outstanding_work_;
    }
    if (!general_operations_.empty()) {
      auto op = std::move(general_operations_.front());
      general_operations_.pop();
      op();
      n++;
      --outstanding_work_;
    }

    while (general_operations_.empty() &&
           (call_operations_.empty() || accept_operations_.empty()) &&
           !stopped_) {
      std::unique_lock lock(wmx_);
      wcv_.wait(lock);
    }
  }
  return n;
}

inline void amc_context::stop()
{
  std::lock_guard lock(omx_);
  stopped_ = true;
  wcv_.notify_all();
}

inline void amc_context::work_started() { ++outstanding_work_; }

inline void amc_context::work_finished()
{
  if (--outstanding_work_ == 0) {
    stop();
  };
}

inline bool amc_context::stopped()
{
  std::lock_guard lock(omx_);
  return stopped_;
}

inline void amc_context::post_general(std::function<void()> op)
{
  std::lock_guard lock(omx_);
  general_operations_.emplace(std::move(op));
  ++outstanding_work_;
  wcv_.notify_all();
}

inline void amc_context::post_call(amc_context::mov_operation& op)
{
  std::lock_guard lock(omx_);
  call_operations_.emplace(std::move(op));
  ++outstanding_work_;
  wcv_.notify_all();
}

inline void amc_context::post_accept(amc_context::ref_operation& op)
{
  std::lock_guard lock(omx_);
  accept_operations_.emplace(std::move(op));
  ++outstanding_work_;
  wcv_.notify_all();
}

template <typename Allocator>
template <typename Function, typename OtherAllocator>
void amc_context::executor<Allocator>::dispatch(Function&& f,
                                                const OtherAllocator& a) const
{
  context_.post_general(f);
}

template <typename Allocator>
template <typename Function, typename OtherAllocator>
void amc_context::executor<Allocator>::post(Function&& f,
                                            const OtherAllocator& a) const
{
  context_.post_general(f);
}

template <typename Allocator>
template <typename Function, typename OtherAllocator>
void amc_context::executor<Allocator>::defer(Function&& f,
                                             const OtherAllocator& a) const
{
  context_.post_general(f);
}

template <typename Allocator>
inline amc_context::executor<Allocator>::executor(amc_context& cntx)
    : context_(cntx)
{
}

}  // namespace amc

#endif  // HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_
