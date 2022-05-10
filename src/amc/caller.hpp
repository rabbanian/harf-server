#ifndef HARF_SRC_AMC_CALLER_HPP_
#define HARF_SRC_AMC_CALLER_HPP_

#include "amc/amc_context.hpp"

namespace amc {

class caller
{
 public:
  typedef amc_context::executor_type executor_type;

  explicit caller(amc_context& cntx) : context_(cntx){};
  caller(const caller&) = delete;
  caller& operator=(const caller&) = delete;

  template <typename Connection, typename Packet, typename CallHandler>
  auto async_call(Connection&& c, Packet&& p, CallHandler&& handler);

  executor_type get_executor() const noexcept;

 private:
  class initiate_async_call;

  amc_context& context_;
};

class caller::initiate_async_call
{
 public:
  typedef amc_context::executor_type executor_type;

  explicit initiate_async_call(caller& self) : self_(self) {}

  executor_type get_executor() const noexcept { return self_.get_executor(); }

  template <typename AcceptHandler, typename Connection, typename Packet>
  void operator()(AcceptHandler&& handler, Connection&& c, Packet&& p) const
  {
    // FIXME: check the type requirement for the AcceptHandler
    // FIXME: convert handler to a non-const-lvalue type first
    amc_context::mov_operation op = {handler, std::move(c), std::move(p)};
    self_.context_.post_call(op);
  }

 private:
  caller& self_;
};

}  // namespace amc

#include "impl/caller.hpp"

#endif  // HARF_SRC_AMC_CALLER_HPP_
