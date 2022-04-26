#include "server.h"

#include <iostream>

#include "internal/connection.h"

net::Server::Server(asio::io_context& ioc, asio::ip::tcp::endpoint& endpoint)
    : asio_acceptor_(ioc), asio_socket_(ioc) {
  std::error_code ec;
  asio_acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    OnError(ec);
    return;
  }

  asio_acceptor_.set_option(asio::socket_base::reuse_address(true), ec);
  if (ec) {
    OnError(ec);
    return;
  }

  asio_acceptor_.bind(endpoint, ec);
  if (ec) {
    OnError(ec);
    return;
  }

  asio_acceptor_.listen(asio::socket_base::max_listen_connections, ec);
  if (ec) {
    OnError(ec);
    return;
  }
}

void net::Server::Run() {
  asio_acceptor_.async_accept(
      asio_socket_,
      [self = shared_from_this()](std::error_code ec) { self->OnAccept(ec); });
}

void net::Server::OnAccept(std::error_code ec) {
  if (ec) {
    OnError(ec);
    return;
  } else {
    std::make_shared<internal::Connection>(std::move(asio_socket_))->Run();
  }

  asio_acceptor_.async_accept(
      asio_socket_,
      [self = shared_from_this()](std::error_code ec) { self->OnAccept(ec); });
}

void net::Server::OnError(std::error_code ec) {
  std::cerr << "Server Error: " << ec.message() << std::endl;
}