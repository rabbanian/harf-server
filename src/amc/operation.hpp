#ifndef HARF_SRC_AMC_OPERATION_HPP_
#define HARF_SRC_AMC_OPERATION_HPP_

#include <functional>

namespace amc {

template <typename Handler, typename Connection, typename Packet>
struct operation
{
  void operator()() { handler_(); }

  Handler handler_;
  Connection connection_;
  Packet packet_;
};

}  // namespace amc

#include "impl/operation.hpp"

#endif  // HARF_SRC_AMC_OPERATION_HPP_
