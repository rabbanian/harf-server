set(PROTO_SRC
        message.proto
        )

protobuf_generate_cpp(SRC INC ${PROTO_SRC})

add_library(protobuf "${SRC};${INC}")
target_include_directories(protobuf PRIVATE ${Protobuf_INCLUDE_DIRS})
target_link_libraries(protobuf "${Protobuf_LIBRARIES}")