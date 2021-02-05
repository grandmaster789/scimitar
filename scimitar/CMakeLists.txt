cmake_minimum_required (VERSION 3.16)

set(SCIMITAR_LIB "scimitar")

project(${SCIMITAR_LIB} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${SCIMITAR_LIB} STATIC
	"scimitar.cpp"
)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(volk CONFIG REQUIRED)

find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")

target_link_libraries(${SCIMITAR_LIB} PRIVATE
	nlohmann_json
	nlohmann_json::nlohmann_json
	glm
	volk::volk
	volk::volk_headers
)

target_include_directories(${SCIMITAR_LIB} PRIVATE
	${VULKAN_HPP_INCLUDE_DIRS}
)