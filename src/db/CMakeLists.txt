set(SRC
        database.cpp
        )

set(INC
        database.h)

set(LIBS
        )

add_library(database OBJECT "${SRC};${INC}")
target_link_libraries(database "${LIBS}")