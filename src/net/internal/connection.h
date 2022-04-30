#ifndef HARF_SRC_NET_INTERNAL_CONNECTION_H_
#define HARF_SRC_NET_INTERNAL_CONNECTION_H_

#include <asio/ip/tcp.hpp>
#include <memory>
#include <vector>

#include "net/packet.h"
#include "utils/queue.h"

namespace net {
class ConnectionManager;
namespace internal {

class Connection : public std::enable_shared_from_this<Connection>
{
 public:
  Connection(asio::ip::tcp::socket socket, net::ConnectionManager &manager);
  ~Connection();
  void Run();
  void Send(net::Packet packet);

 private:
  void OnHeaderRead(std::error_code ec, std::size_t bytes);
  void OnBodyRead(std::error_code ec, std::size_t bytes);
  void OnError(std::error_code ec);
  void OnWrite(std::error_code ec, std::size_t bytes);

  asio::ip::tcp::socket asio_socket_;
  net::Packet packet_;
  net::ConnectionManager &manager_;
};

}  // namespace internal
}  // namespace net

#endif  // HARF_SRC_NET_INTERNAL_CONNECTION_H_
