#include "connection.h"

#include <asio/read.hpp>
#include <asio/write.hpp>
#include <iostream>

#include "net/connection_manager.h"

net::internal::Connection::Connection(asio::ip::tcp::socket socket,
                                      net::ConnectionManager &manager)
    : asio_socket_(std::move(socket)), manager_(manager)
{
  manager_.Add(*this);
}

void net::internal::Connection::Run()
{
  asio::async_read(
      asio_socket_, read_packet_.GetHeader(),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnHeaderRead(ec, bytes);
      });
}

void net::internal::Connection::OnHeaderRead(std::error_code ec,
                                             std::size_t bytes)
{
  if (ec) {
    OnError(ec);
    return;
  }

  asio::async_read(
      asio_socket_, read_packet_.GetBody(),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnBodyRead(ec, bytes);
      });
}

void net::internal::Connection::OnBodyRead(std::error_code ec,
                                           std::size_t bytes)
{
  if (ec) {
    OnError(ec);
    return;
  }

  manager_.CallMethod(shared_from_this(), std::move(read_packet_));
  // TODO: What happens to the object life cycle?

  asio::async_read(
      asio_socket_, read_packet_.GetHeader(),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnHeaderRead(ec, bytes);
      });
}

void net::internal::Connection::OnWrite(std::error_code ec, std::size_t bytes)
{
  if (ec) {
    OnError(ec);
    return;
  }

  std::lock_guard lock(mx_);
  write_packets_.pop();
  if (write_packets_.empty()) return;

  asio::async_write(
      asio_socket_, write_packets_.front().GetBuffer(),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnWrite(ec, bytes);
      });
}

void net::internal::Connection::OnError(std::error_code ec)
{
  std::cerr << "Connection Error: " << ec.message() << std::endl;
}

void net::internal::Connection::Send(net::Packet packet)
{
  std::lock_guard lock(mx_);
  write_packets_.emplace(std::move(packet));
  if (write_packets_.size() > 1) return;

  asio::async_write(
      asio_socket_, write_packets_.front().GetBuffer(),
      [self = shared_from_this()](std::error_code ec, std::size_t bytes) {
        self->OnWrite(ec, bytes);
      });
}

net::internal::Connection::~Connection() { manager_.Remove(*this); }
