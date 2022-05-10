#ifndef HARF_SRC_AMC_IMPL_ACCEPTOR_HPP_
#define HARF_SRC_AMC_IMPL_ACCEPTOR_HPP_

#include <asio/async_result.hpp>

namespace amc {

template <typename Connection, typename Packet, typename AcceptHandler>
auto acceptor::async_accept(Connection& c, Packet& p, AcceptHandler&& handler)
{
  return asio::async_initiate<AcceptHandler, void()>(
      initiate_async_accept(*this), handler, c, p);
}

inline acceptor::executor_type acceptor::get_executor() const noexcept
{
  return context_.get_executor();
}

}  // namespace amc

#endif  // HARF_SRC_AMC_IMPL_ACCEPTOR_HPP_
