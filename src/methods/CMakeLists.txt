set(SRC
	echo.cpp
	server.cpp
	)

set(INC
	echo.h
	server.h
	)

set(LIBS
	network
	command
	)

add_library(methods OBJECT "${SRC};${INC}")
target_link_libraries(methods "${LIBS}")
