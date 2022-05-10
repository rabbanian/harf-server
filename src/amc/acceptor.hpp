#ifndef HARF_SRC_AMC_ACCEPTOR_HPP_
#define HARF_SRC_AMC_ACCEPTOR_HPP_

#include "amc_context.hpp"

namespace amc {

class acceptor
{
 public:
  typedef amc_context::executor_type executor_type;

  explicit acceptor(amc_context& cntx) : context_(cntx) {}
  acceptor(const acceptor&) = delete;
  acceptor& operator=(const acceptor&) = delete;

  template <typename Connection, typename Packet, typename AcceptHandler>
  auto async_accept(Connection& c, Packet& p, AcceptHandler&& handler);

  executor_type get_executor() const noexcept;

 private:
  class initiate_async_accept;

  amc_context& context_;
};

class acceptor::initiate_async_accept
{
 public:
  typedef amc_context::executor_type executor_type;

  explicit initiate_async_accept(acceptor& self) : self_(self) {}

  executor_type get_executor() const noexcept { return self_.get_executor(); }

  template <typename AcceptHandler, typename Connection, typename Packet>
  void operator()(AcceptHandler&& handler, Connection& c, Packet& p)
  {
    // FIXME: check the type requirement for the AcceptHandler
    // FIXME: convert handler to a non-const-lvalue type first
    amc_context::ref_operation op = {handler, c, p};
    self_.context_.post_accept(op);
  }

 private:
  acceptor& self_;
};

}  // namespace amc

#include "impl/acceptor.hpp"

#endif  // HARF_SRC_AMC_ACCEPTOR_HPP_
