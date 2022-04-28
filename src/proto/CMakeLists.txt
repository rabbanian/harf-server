set(PROTO_SRC
	message.proto
	header.proto
	)

protobuf_generate_cpp(SRC INC ${PROTO_SRC})

add_library(protobuf "${SRC};${INC}")
target_link_libraries(protobuf "${Protobuf_LIBRARIES}")