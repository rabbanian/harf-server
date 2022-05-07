#ifndef HARF_SRC_METHODS_SERVER_H_
#define HARF_SRC_METHODS_SERVER_H_

#include <memory>

#include "cmd/cmd_acceptor.hpp"
#include "cmd/cmd_context.hpp"
#include "net/connection_manager.h"

namespace methods {

class Server : public std::enable_shared_from_this<Server>
{
 public:
  Server(cmd::cmd_context& cntx, net::ConnectionManager& manager);
  void Run();

 private:
  void OnPacket();
  void OnError(std::error_code ec);

  cmd::cmd_context& cmd_context_;
  cmd::cmd_acceptor cmd_acceptor_;
  net::Packet packet_;
  std::weak_ptr<net::internal::Connection> connection_;
};

}  // namespace methods

#endif  // HARF_SRC_METHODS_SERVER_H_
