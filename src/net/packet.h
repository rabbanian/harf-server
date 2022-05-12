#ifndef HARF_SRC_NET_INTERNAL_PACKET_H_
#define HARF_SRC_NET_INTERNAL_PACKET_H_

#include <flatbuffers/detached_buffer.h>

#include <array>
#include <asio/buffer.hpp>
#include <cstddef>
#include <cstdint>

namespace net {

class Packet
{
 public:
  Packet() = default;
  explicit Packet(flatbuffers::DetachedBuffer from);
  Packet(const Packet& from) = delete;
  Packet(Packet&& from) noexcept;

  Packet& operator=(const Packet& from) = delete;
  Packet& operator=(Packet&& from) noexcept;

  // TODO: replace Asio types with a more generic std types
  asio::mutable_buffer GetHeader();
  asio::mutable_buffer GetBody();
  std::array<asio::const_buffer, 2> GetBuffer() const;
  std::uint32_t GetType() const;

 private:
  struct Header
  {
    std::uint32_t size;
    std::uint32_t type;
  } header_ = {};
  flatbuffers::DetachedBuffer payload_;
};

}  // namespace net

#endif  // HARF_SRC_NET_INTERNAL_PACKET_H_
