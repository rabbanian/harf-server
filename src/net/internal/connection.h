#ifndef HARF_SRC_NET_INTERNAL_CONNECTION_H_
#define HARF_SRC_NET_INTERNAL_CONNECTION_H_

#include <asio/ip/tcp.hpp>
#include <memory>
#include <vector>

#include "utils/queue.h"

namespace net::internal {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(asio::ip::tcp::socket socket);
  void Run();

 private:
  void OnError(std::error_code ec);
  void OnRead(std::error_code ec, std::size_t bytes);
  void OnWrite(std::error_code ec, std::size_t bytes);

  asio::ip::tcp::socket asio_socket_;
  std::vector<char> buffer_;
};

}  // namespace net::internal

#endif  // HARF_SRC_NET_INTERNAL_CONNECTION_H_
