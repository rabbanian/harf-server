#ifndef HARF_SRC_METHODS_ECHO_H_
#define HARF_SRC_METHODS_ECHO_H_

#include <memory>

#include "net/internal/connection.h"
#include "net/packet.h"

namespace methods {

class Echo : public std::enable_shared_from_this<Echo>
{
 public:
  Echo(std::weak_ptr<net::internal::Connection> connection, net::Packet packet);
  void Run();

 private:
  std::weak_ptr<net::internal::Connection> connection_;
  net::Packet packet_;
};

}  // namespace methods

#endif  // HARF_SRC_METHODS_ECHO_H_
