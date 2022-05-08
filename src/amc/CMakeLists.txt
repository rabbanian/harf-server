set(INC
	amc_context.hpp
	acceptor.hpp
	scheduler.hpp

	impl/amc_context.hpp
	impl/acceptor.hpp
	impl/scheduler.hpp
	)

add_library(amc INTERFACE "${INC}")
