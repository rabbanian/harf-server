#ifndef HARF_SRC_CMD_CMD_ACCEPTOR_HPP_
#define HARF_SRC_CMD_CMD_ACCEPTOR_HPP_

#include "cmd_context.hpp"

namespace cmd {

class cmd_acceptor
{
 public:
  explicit cmd_acceptor(cmd_context& cntx) : context_(cntx) {}

  template <typename Connection, typename Packet, typename AcceptHandler>
  void async_accept(Connection c, Packet p, AcceptHandler&& handler)
  {
  }

 private:
  cmd_context& context_;
};

}  // namespace cmd

#include "impl/cmd_acceptor.hpp"

#endif  // HARF_SRC_CMD_CMD_ACCEPTOR_HPP_
