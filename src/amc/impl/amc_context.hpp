#ifndef HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_
#define HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_

#include <iostream>

namespace amc {

inline amc_context::executor_type amc_context::get_executor() noexcept
{
  return executor_type(*this);
}

inline std::size_t amc_context::run()
{
  std::cout << "run" << std::endl;
  return 0;
}

inline void amc_context::stop() {}

inline void amc_context::post_immediate_completion()
{
  std::cout << "A job has came!" << std::endl;
}

template <typename Function, typename OtherAllocator>
void amc_context::executor::dispatch(Function&& f,
                                     const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

template <typename Function, typename OtherAllocator>
void amc_context::executor::post(Function&& f, const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

template <typename Function, typename OtherAllocator>
void amc_context::executor::defer(Function&& f, const OtherAllocator& a) const
{
  context_.post_immediate_completion();
}

inline amc_context::executor::executor(amc_context& cntx) : context_(cntx) {}

}  // namespace amc

#endif  // HARF_SRC_AMC_IMPL_AMC_CONTEXT_HPP_
