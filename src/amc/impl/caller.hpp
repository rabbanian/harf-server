#ifndef HARF_SRC_AMC_IMPL_CALLER_HPP_
#define HARF_SRC_AMC_IMPL_CALLER_HPP_

#include <asio/async_result.hpp>

namespace amc {

template <typename Connection, typename Packet, typename CallHandler>
auto caller::async_call(Connection&& c, Packet&& p, CallHandler&& handler)
{
  return asio::async_initiate<CallHandler, void()>(initiate_async_call(*this),
                                                   handler, c, std::move(p));
}

inline caller::executor_type caller::get_executor() const noexcept
{
  return context_.get_executor();
}

}  // namespace amc

#endif  // HARF_SRC_AMC_IMPL_CALLER_HPP_
