set(SRC
        database.cpp
        )

set(INC
        database.h)

set(LIBS
        )

add_library(database OBJECT "${SRC};${INC}")
target_include_directories(database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(database "${LIBS}")