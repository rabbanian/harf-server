#ifndef HARF_SRC_NET_CONNECTION_MANAGER_H_
#define HARF_SRC_NET_CONNECTION_MANAGER_H_

#include <memory>
#include <unordered_set>

#include "net/internal/connection.h"
#include "net/packet.h"
#include "utils/queue.h"

namespace net {

class ConnectionManager
{
 public:
  explicit ConnectionManager(
      utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>,
                             net::Packet>> &queue);
  utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>, net::Packet>>
      &GetQueue();
  void Add(net::internal::Connection &con);
  void Remove(net::internal::Connection &con);
  void SendToAll(const net::Packet &packet) const;

 private:
  std::unordered_set<net::internal::Connection *> connections_;
  utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>, net::Packet>>
      &queue_;
};

}  // namespace net

#endif  // HARF_SRC_NET_CONNECTION_MANAGER_H_
