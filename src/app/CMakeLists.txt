
set(SRC
	test.cpp
	)

set(INC
	)

set(LIBS
	network
	)

add_executable(${PROJECT_NAME} "${SRC};${INC}")
target_link_libraries(${PROJECT_NAME} "${LIBS}")
