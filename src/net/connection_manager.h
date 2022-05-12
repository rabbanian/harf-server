#ifndef HARF_SRC_NET_CONNECTION_MANAGER_H_
#define HARF_SRC_NET_CONNECTION_MANAGER_H_

#include <memory>
#include <unordered_set>

#include "amc/amc_context.hpp"
#include "amc/caller.hpp"
#include "net/internal/connection.h"
#include "net/packet.h"

namespace net {

class ConnectionManager
{
 public:
  explicit ConnectionManager(amc::amc_context &ctx);
  void Add(net::internal::Connection &con);
  void Remove(net::internal::Connection &con);
  void CallMethod(std::weak_ptr<net::internal::Connection> &&c,
                  net::Packet &&p);

 private:
  std::unordered_set<net::internal::Connection *> connections_;
  amc::caller amc_caller_;
};

}  // namespace net

#endif  // HARF_SRC_NET_CONNECTION_MANAGER_H_
