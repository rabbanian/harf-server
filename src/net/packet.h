#ifndef HARF_SRC_NET_INTERNAL_PACKET_H_
#define HARF_SRC_NET_INTERNAL_PACKET_H_

#include <google/protobuf/message.h>

#include <array>
#include <asio/buffer.hpp>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace net {

class Packet
{
 public:
  Packet() = default;
  explicit Packet(const google::protobuf::Message* message);
  Packet(const Packet& from) = default;
  Packet(Packet&& from) noexcept;

  Packet& operator=(Packet&& from) noexcept;
  Packet& operator=(Packet& from) = default;

  // TODO: replace Asio types with a more generic std types
  asio::mutable_buffer GetHeader();
  asio::mutable_buffer GetBody();
  std::vector<asio::const_buffer> GetBuffer();

 private:
  struct Header
  {
    std::uint32_t size;
    std::uint32_t type;
  } header_ = {};
  std::vector<std::uint8_t> payload_;
};

}  // namespace net

#endif  // HARF_SRC_NET_INTERNAL_PACKET_H_
