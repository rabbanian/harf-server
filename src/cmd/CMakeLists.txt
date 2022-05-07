set(INC
	cmd_context.hpp
	cmd_acceptor.hpp

	impl/cmd_context.hpp
	)

add_library(command INTERFACE "${INC}")
