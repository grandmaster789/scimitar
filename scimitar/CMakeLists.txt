cmake_minimum_required (VERSION 3.16)

set(SCIMITAR_LIB "scimitar_lib")

project(${SCIMITAR_LIB} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${SCIMITAR_LIB} STATIC
	"scimitar.cpp"
	"os/window.h"
	"os/window.cpp"
	"core/engine.h"
	"core/engine.cpp"
	"util/algorithm.h"
	"core/system.h"
	"core/system.cpp"
	"dependencies.h" 
	"dependencies.cpp"
	"preprocessor.h" 
)

# vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(volk CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")

# vulkan and shaderc are part of the Vulkan SDK 
find_package(Vulkan REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)

# extract the VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
endif()

target_link_libraries(${SCIMITAR_LIB} 
PUBLIC
	glfw
	Vulkan::Vulkan
	Microsoft.GSL::GSL

PRIVATE
	nlohmann_json
	nlohmann_json::nlohmann_json	
	volk::volk
	volk::volk_headers
)

target_include_directories(${SCIMITAR_LIB} PRIVATE
	${VULKAN_HPP_INCLUDE_DIRS}
	${VULKAN_PATH}/Include
)