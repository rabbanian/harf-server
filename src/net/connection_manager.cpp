#include "connection_manager.h"

net::ConnectionManager::ConnectionManager(amc::amc_context& ctx)
    : amc_caller_(ctx)
{
}

void net::ConnectionManager::Add(net::internal::Connection& con)
{
  connections_.insert(&con);
}

void net::ConnectionManager::Remove(net::internal::Connection& con)
{
  connections_.erase(&con);
}

void net::ConnectionManager::SendToAll(const net::Packet& packet) const
{
  for (auto con : connections_) {
    con->Send(packet);
  }
}

void net::ConnectionManager::CallMethod(
    std::weak_ptr<net::internal::Connection>&& c, net::Packet&& p)
{
  amc_caller_.async_call(c, p, []() {});
}
