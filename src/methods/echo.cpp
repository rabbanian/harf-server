#include "echo.h"

methods::Echo::Echo(std::weak_ptr<net::internal::Connection> connection,
                    net::Packet packet)
    : connection_(std::move(connection)), packet_(std::move(packet))
{
}

void methods::Echo::Run()
{
  if (!connection_.expired()) {
    auto sp = connection_.lock();
    sp->Send(std::move(packet_));
  }
}
