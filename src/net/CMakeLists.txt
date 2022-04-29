set(SRC
	server.cpp
	packet.cpp
	connection_manager.cpp

	internal/connection.cpp
	)

set(INC
	server.h
	packet.h
	connection_manager.h

	internal/connection.h
	)

set(LIBS
	protobuf
	)

add_library(network "${SRC};${INC}")
target_link_libraries(network "${LIBS}")