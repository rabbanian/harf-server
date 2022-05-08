
set(SRC
	main.cpp
	)

set(INC
	)

set(LIBS
	network
	methods
	)

add_executable(${PROJECT_NAME} "${SRC};${INC}")
target_link_libraries(${PROJECT_NAME} "${LIBS}")
