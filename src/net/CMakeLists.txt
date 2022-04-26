set(SRC
	server.cpp

	internal/connection.cpp
	)

set(INC
	server.h

	internal/connection.h
	)

set(LIBS
	)

add_library(network "${SRC};${INC}")
target_link_libraries(network "${LIBS}")