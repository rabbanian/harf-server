#include "server.h"

#include "echo.h"

methods::Server::Server(amc::amc_context& exc, net::ConnectionManager& manager)
    : amc_context_(exc), amc_acceptor_(exc)
{
}

void methods::Server::Run()
{
  amc_acceptor_.async_accept(
      connection_, packet_,
      [self = shared_from_this()]() { self->OnPacket(); });
}

void methods::Server::OnPacket()
{
  if (packet_.GetType() == 01) {
    std::make_shared<methods::Echo>(connection_, std::move(packet_))->Run();
  }

  amc_acceptor_.async_accept(
      connection_, packet_,
      [self = shared_from_this()]() { self->OnPacket(); });
}

void methods::Server::OnError(std::error_code ec)
{
  std::cerr << "Server Error: " << ec.message() << std::endl;
}
