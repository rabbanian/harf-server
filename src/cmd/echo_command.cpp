#include "echo_command.h"

void cmd::EchoCommand::Execute(
    std::weak_ptr<net::internal::Connection> connection, net::Packet packet)
{
  if (!connection.expired()) {
    auto sp = connection.lock();
    sp->Send(packet);
  }
}
