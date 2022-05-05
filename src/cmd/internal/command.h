#ifndef HARF_SRC_CMD_INTERNAL_COMMAND_H_
#define HARF_SRC_CMD_INTERNAL_COMMAND_H_

#include "net/internal/connection.h"
#include "net/packet.h"

namespace cmd::internal {

class Command
{
 public:
  virtual void Execute(std::weak_ptr<net::internal::Connection>,
                       net::Packet) = 0;
};

}  // namespace cmd::internal

#endif  // HARF_SRC_CMD_INTERNAL_COMMAND_H_
