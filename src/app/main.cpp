#include <asio/signal_set.hpp>
#include <thread>

#include "net/connection_manager.h"
#include "net/server.h"
#include "proto/echo.pb.h"
#include "utils/queue.h"

int main()
{
  asio::io_context ioc;
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 1234);

  utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>, net::Packet>>
      queue;
  net::ConnectionManager manager(queue);
  std::make_shared<net::Server>(ioc, endpoint, manager)->Run();

  asio::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&ioc](std::error_code const&, int) { ioc.stop(); });

  std::thread([&ioc]() { ioc.run(); }).detach();

  while (queue.Wait()) {
    auto [connection, packet] = queue.Pop();
    if (!connection.expired()) {
      auto sp = connection.lock();
      sp->Send(packet);
    }
  }
  return 0;
}
