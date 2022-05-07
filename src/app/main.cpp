#include <asio/io_context.hpp>
#include <asio/signal_set.hpp>
#include <thread>

#include "cmd/cmd_context.hpp"
#include "methods/server.h"
#include "net/connection_manager.h"
#include "net/server.h"

int main()
{
  asio::io_context ioc;
  cmd::cmd_context cmdc;

  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 1234);

  utils::Queue<std::pair<std::weak_ptr<net::internal::Connection>, net::Packet>>
      queue;
  net::ConnectionManager manager(queue);
  std::make_shared<net::Server>(ioc, endpoint, manager)->Run();
  std::make_shared<methods::Server>(cmdc, manager)->Run();

  asio::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&ioc, &cmdc](std::error_code const&, int) {
    ioc.stop();
    cmdc.stop();
  });

  auto th = std::thread([&ioc]() { ioc.run(); });
  cmdc.run();

  th.join();
  return 0;
}
