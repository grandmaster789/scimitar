cmake_minimum_required (VERSION 3.16) # some of the dependencies have much more strict requirements

set(SCIMITAR_APP "scimitar")

project(${SCIMITAR_APP} LANGUAGES CXX)

add_executable(
	${SCIMITAR_APP}
	"main.cpp"
)

include_directories(../scimitar)

target_link_libraries(${SCIMITAR_APP} PRIVATE
	scimitar_lib
)

target_include_directories(${SCIMITAR_APP} PRIVATE
	"../scimitar"
)

set_target_properties(${SCIMITAR_APP} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS OFF
)
