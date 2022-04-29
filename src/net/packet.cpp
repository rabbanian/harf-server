#include "packet.h"

net::Packet::Packet(const google::protobuf::Message *message)
{
  auto size = message->ByteSizeLong();
  header_.size = size;
  header_.type = 0x00'00'00'00;
  payload_.resize(size);
  message->SerializeWithCachedSizesToArray(payload_.data());
}

net::Packet::Packet(net::Packet &&from) noexcept : Packet()
{
  *this = std::move(from);
}

net::Packet &net::Packet::operator=(net::Packet &&from) noexcept
{
  if (this == &from) return *this;
  payload_ = std::move(from.payload_);
  header_ = from.header_;
  from.header_ = {0};

  return *this;
}

asio::mutable_buffer net::Packet::GetHeader()
{
  return asio::buffer(&header_, sizeof(header_));
}

asio::mutable_buffer net::Packet::GetBody()
{
  payload_.resize(header_.size);
  return asio::buffer(payload_);
}

std::vector<asio::const_buffer> net::Packet::GetBuffer()
{
  return {asio::buffer(&header_, sizeof(header_)), asio::buffer(payload_)};
}