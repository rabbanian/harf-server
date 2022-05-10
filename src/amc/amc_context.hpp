#ifndef HARF_SRC_AMC_AMC_CONTEXT_HPP_
#define HARF_SRC_AMC_AMC_CONTEXT_HPP_

#include <asio/execution_context.hpp>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

#include "amc/operation.hpp"
#include "net/internal/connection.h"
#include "net/packet.h"

namespace amc {

class amc_context : public asio::execution_context
{
 public:
  template <typename Allocator>
  class executor;

  template <typename Allocator>
  friend class executor;

  typedef executor<std::allocator<void>> executor_type;
  typedef operation<std::function<void()>,
                    std::weak_ptr<net::internal::Connection>, net::Packet>
      mov_operation;
  typedef operation<std::function<void()>,
                    std::weak_ptr<net::internal::Connection>&, net::Packet&>
      ref_operation;

  amc_context();
  amc_context(const amc_context&) = delete;
  amc_context& operator=(const amc_context&) = delete;

  executor_type get_executor() noexcept;

  std::size_t run();
  void stop();
  bool stopped();

  void post_call(mov_operation& op);
  void post_accept(ref_operation& op);

 private:
  void post_general(std::function<void()> op);

  void work_started();
  void work_finished();

  std::mutex omx_;
  std::mutex wmx_;
  std::condition_variable wcv_;
  std::queue<mov_operation> call_operations_;
  std::queue<ref_operation> accept_operations_;
  std::queue<std::function<void()>> general_operations_;

  bool stopped_;
  std::atomic<long> outstanding_work_;
};

template <typename Allocator>
class amc_context::executor
{
 public:
  executor(const executor& from) noexcept : context_(from.context_)
  {
    context_.work_started();
  }

  executor(executor&& from) noexcept : context_(from.context_)
  {
    *this = std::move(from);
  }

  ~executor() noexcept { context_.work_finished(); }

  executor& operator=(const executor& from) noexcept
  {
    context_.work_started();
    return *this;
  }
  executor& operator=(executor&& from) noexcept
  {
    if (this == &from) return *this;

    context_ = from.context_;

    return *this;
  }

  bool operator==(const executor& other) const
  {
    return &context_ == &other.context_;
  }
  bool operator!=(const executor& other) const { return !(*this == other); }

  amc_context& context() const noexcept { return context_; }
  void on_work_started() const noexcept { context_.work_started(); }
  void on_work_finished() const noexcept { context_.work_finished(); }

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
