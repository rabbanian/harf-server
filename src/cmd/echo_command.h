#ifndef HARF_SRC_CMD_ECHO_COMMAND_H_
#define HARF_SRC_CMD_ECHO_COMMAND_H_

#include "cmd/internal/command.h"

namespace cmd {

class EchoCommand : public internal::Command
{
 public:
  void Execute(std::weak_ptr<net::internal::Connection> connection,
               net::Packet packet) override;
};

}  // namespace cmd

#endif  // HARF_SRC_CMD_ECHO_COMMAND_H_
