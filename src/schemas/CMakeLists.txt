set(SRC
	echo.fbs
	)

build_flatbuffers("${SRC}" "${CMAKE_CURRENT_SOURCE_DIR}" generated_schemas "" "${CMAKE_CURRENT_BINARY_DIR}" "" "")

add_library(schemas INTERFACE)
add_dependencies(schemas generated_schemas)