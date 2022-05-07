#include "server.h"

#include "echo.h"

methods::Server::Server(cmd::cmd_context& exc, net::ConnectionManager& manager)
    : cmd_context_(exc), cmd_acceptor_(exc)
{
}

void methods::Server::Run()
{
  cmd_acceptor_.async_accept(
      connection_, packet_,
      [self = shared_from_this()]() { self->OnPacket(); });
}

void methods::Server::OnPacket()
{
  if (packet_.GetType() == 01) {
    std::make_shared<methods::Echo>(connection_, std::move(packet_))->Run();
  }

  cmd_acceptor_.async_accept(
      connection_, packet_,
      [self = shared_from_this()]() { self->OnPacket(); });
}

void methods::Server::OnError(std::error_code ec)
{
  std::cerr << "Server Error: " << ec.message() << std::endl;
}
