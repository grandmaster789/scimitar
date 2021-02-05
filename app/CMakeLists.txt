cmake_minimum_required (VERSION 3.16) # some of the dependencies have much more strict requirements

set(SCIMITAR_APP "scimitar_application")

project(${SCIMITAR_APP} LANGUAGES CXX)

add_executable(
	${SCIMITAR_APP}
	"main.cpp"
)

include_directories(../scimitar)

find_package(glm CONFIG REQUIRED)

target_link_libraries(${SCIMITAR_APP} PRIVATE
	scimitar
	glm
)

target_include_directories(${SCIMITAR_APP} PRIVATE
	"../scimitar"
)

set_target_properties(${SCIMITAR_APP} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS OFF
)
