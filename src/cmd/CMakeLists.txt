set(SRC
	echo_command.cpp
	)

set(INC
	echo_command.h

	internal/command.h
	)

set(LIBS
	network
	)

add_library(commands OBJECT "${SRC};${INC}")
target_link_libraries(commands "${LIBS}")