#include <asio/io_context.hpp>
#include <asio/signal_set.hpp>
#include <thread>

#include "amc/amc_context.hpp"
#include "methods/server.h"
#include "net/connection_manager.h"
#include "net/server.h"

int main()
{
  asio::io_context ioc;
  amc::amc_context mcc;

  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 1234);

  net::ConnectionManager manager(mcc);
  std::make_shared<net::Server>(ioc, endpoint, manager)->Run();
  std::make_shared<methods::Server>(mcc, manager)->Run();

  asio::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&ioc, &mcc](std::error_code const&, int) {
    ioc.stop();
    mcc.stop();
  });

  auto th = std::thread([&ioc]() { ioc.run(); });
  mcc.run();

  th.join();
  return 0;
}
