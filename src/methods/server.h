#ifndef HARF_SRC_METHODS_SERVER_H_
#define HARF_SRC_METHODS_SERVER_H_

#include <memory>

#include "amc/acceptor.hpp"
#include "amc/amc_context.hpp"
#include "net/connection_manager.h"

namespace methods {

class Server : public std::enable_shared_from_this<Server>
{
 public:
  Server(amc::amc_context& cntx, net::ConnectionManager& manager);
  void Run();

 private:
  void OnPacket();
  void OnError(std::error_code ec);

  amc::amc_context& amc_context_;
  amc::acceptor amc_acceptor_;
  net::Packet packet_;
  std::weak_ptr<net::internal::Connection> connection_;
};

}  // namespace methods

#endif  // HARF_SRC_METHODS_SERVER_H_
