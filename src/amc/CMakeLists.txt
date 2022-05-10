set(INC
	amc_context.hpp
	acceptor.hpp
	caller.hpp
	operation.hpp

	impl/amc_context.hpp
	impl/acceptor.hpp
	impl/caller.hpp
	impl/operation.hpp
	)

add_library(amc INTERFACE "${INC}")
