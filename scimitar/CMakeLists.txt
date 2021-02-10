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
	"util/typemap.h" 
	"app/application.h" 
	"app/application.cpp"
	"renderer/renderer.h" 
	"renderer/renderer.cpp" 
	"os/os.h" 
	"os/os.cpp"
	"compile_options.h" 
	"input/input.cpp" 
	"input/input.h" 
	"util/function.h" 
	"util/spin_mutex.h" 
	"util/spin_mutex.cpp"
       "input/keyboard.h" "input/keyboard.cpp" "input/mouse.h" "input/mouse.cpp" "core/mediator.h" "core/detail/mediator_queue.h")

# vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)

# vulkan and shaderc are part of the Vulkan SDK 
find_package(Vulkan REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)

target_link_libraries(${SCIMITAR_LIB} 
PUBLIC
	glfw
	Vulkan::Vulkan
	Microsoft.GSL::GSL

PRIVATE
	nlohmann_json
	nlohmann_json::nlohmann_json
	unofficial::concurrentqueue::concurrentqueue
	${SHADERC_LIB}
)

target_include_directories(${SCIMITAR_LIB} PRIVATE
	${Vulkan_INCLUDE_DIRS}
)

