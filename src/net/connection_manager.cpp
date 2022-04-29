#include "connection_manager.h"

#include "internal/connection.h"

net::ConnectionManager::ConnectionManager(
    utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>,
                           net::Packet>>& queue)
    : queue_(queue)
{
}

utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>, net::Packet>>&
net::ConnectionManager::GetQueue()
{
  return queue_;
}

void net::ConnectionManager::Add(net::internal::Connection& con)
{
  connections_.insert(&con);
  std::cout << "New Connection!" << std::endl;
}

void net::ConnectionManager::Remove(net::internal::Connection& con)
{
  connections_.erase(&con);
  std::cout << "Connection Gone!" << std::endl;
}

void net::ConnectionManager::SendToAll(const net::Packet& packet) const
{
  for (auto con : connections_) {
    con->Send(packet);
  }
}
