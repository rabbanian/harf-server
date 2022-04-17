include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(application)
add_subdirectory(db)
add_subdirectory(protobuf)