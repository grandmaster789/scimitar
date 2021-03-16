cmake_minimum_required (VERSION 3.16)

set(SCIMITAR_LIB "scimitar_lib")

project(${SCIMITAR_LIB} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${SCIMITAR_LIB} STATIC
	"scimitar.cpp"

	"compile_options.h" 
	"dependencies.h" 
	"dependencies.cpp"
	"preprocessor.h" 
	"app/application.h" 
	"app/application.cpp"
	"core/engine.h"
	"core/engine.cpp"
	"core/mediator.h" 
	"core/system.h"
	"core/system.cpp"
	"core/detail/mediator_queue.h" 
	"os/os.h" 
	"os/os.cpp"
	"os/window.h"
	"os/window.cpp"
	"renderer/renderer.h" 
	"renderer/renderer.cpp" 
	"input/input.cpp" 
	"input/input.h" 
	"input/keyboard.h" 
	"input/keyboard.cpp" 
	"input/mouse.h" 
	"input/mouse.cpp" 
	"math/helpers.h" 
	"util/algorithm.h"
	"util/function.h" 
	"util/literals.h" 
	"util/spin_mutex.h" 
	"util/spin_mutex.cpp"
	"util/string_util.h" 
	"util/string_util.cpp"
	"util/typemap.h" 
	"util/debugger.h"
	"util/debugger.cpp" 
	"util/concepts.h" 
	"util/codec/utf.h" 
	"util/codec/utf.cpp" 
	"util/codec/codepage_1252.h" 
	"util/codec/codepage_1252.cpp"
 "util/endian.h" "util/memory.h"  "gui/widget.h" "util/biguint.h" "util/codec/base_n.h" "util/codec/base_n.cpp" )

# vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# vulkan and shaderc are part of the Vulkan SDK 
find_package(Vulkan REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)

target_link_libraries(${SCIMITAR_LIB} 
PUBLIC
	Vulkan::Vulkan
	Microsoft.GSL::GSL
	fmt::fmt

PRIVATE
	nlohmann_json
	nlohmann_json::nlohmann_json
	unofficial::concurrentqueue::concurrentqueue
	${SHADERC_LIB}
)

target_include_directories(${SCIMITAR_LIB} PRIVATE
	${Vulkan_INCLUDE_DIRS}
)

