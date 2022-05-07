#ifndef HARF_SRC_CMD_IMPL_CMD_CONTEXT_HPP_
#define HARF_SRC_CMD_IMPL_CMD_CONTEXT_HPP_

#include <iostream>

namespace cmd {

inline cmd_context::executor_type cmd_context::get_executor() noexcept
{
  return executor_type(*this);
}

inline std::size_t cmd_context::run()
{
  std::cout << "run" << std::endl;
  return 0;
}

inline void cmd_context::stop() {}

inline void cmd_context::post_immediate_completion()
{
  std::cout << "A job has came!" << std::endl;
}

template <typename Function, typename OtherAllocator>
void cmd_context::executor::dispatch(Function&& f,
                                     const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

template <typename Function, typename OtherAllocator>
void cmd_context::executor::post(Function&& f, const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

template <typename Function, typename OtherAllocator>
void cmd_context::executor::defer(Function&& f, const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

inline cmd_context::executor::executor(cmd_context& cntx) : context_(cntx) {}

}  // namespace cmd

#endif  // HARF_SRC_CMD_IMPL_CMD_CONTEXT_HPP_
