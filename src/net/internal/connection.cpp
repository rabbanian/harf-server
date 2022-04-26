#include "connection.h"

#include <asio/read.hpp>
#include <asio/write.hpp>
#include <iostream>

net::internal::Connection::Connection(asio::ip::tcp::socket socket)
    : asio_socket_(std::move(socket)), buffer_(10) {}

void net::internal::Connection::Run() {
  asio::async_read(
      asio_socket_, asio::buffer(buffer_),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnRead(ec, bytes);
      });
}

void net::internal::Connection::OnRead(std::error_code ec, std::size_t bytes) {
  if (ec) {
    OnError(ec);
    return;
  }
  std::cout << "bytes read: " << bytes << std::endl;
  for (auto c : buffer_) std::cout << c;

  asio::async_write(
      asio_socket_, asio::buffer(buffer_),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnWrite(ec, bytes);
      });
}

void net::internal::Connection::OnWrite(std::error_code ec, std::size_t bytes) {
  if (ec) {
    OnError(ec);
    return;
  }

  asio::async_read(
      asio_socket_, asio::buffer(buffer_),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnRead(ec, bytes);
      });
}

void net::internal::Connection::OnError(std::error_code ec) {
  std::cerr << "Connection Error: " << ec.message() << std::endl;
}
