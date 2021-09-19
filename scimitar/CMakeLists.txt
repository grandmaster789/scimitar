cmake_minimum_required (VERSION 3.16)

set(SCIMITAR_LIB "scimitar_lib")

project(${SCIMITAR_LIB} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${SCIMITAR_LIB} STATIC
	"scimitar.h"
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
	"core/logger.h" 
	"core/logger.cpp"
	"core/logger/log_category.h" 
	"core/logger/log_category.cpp"  
	"core/logger/log_message.h" 
	"core/logger/log_message.cpp" 
	"core/logger/log_sink.h" 
	"core/logger/log_sink.cpp"

	"geometry/extent_2d.h" 
	"geometry/extent_2d.cpp"
	"geometry/aa_rectangle.h" 
	"geometry/aa_rectangle.cpp" 
	"geometry/point_2d.h"
	"geometry/point_2d.cpp"
	"geometry/direction_2d.h" 
	"geometry/direction_2d.cpp" 

	"os/os.h" 
	"os/os.cpp"
	$<${SCIMITAR_WINDOWS}:${CMAKE_CURRENT_SOURCE_DIR}/os/os_win32.cpp>
	"os/vma_impl.cpp"
	"os/render_device.h" 
	"os/render_device.cpp" 
	"os/gpu_queue.h"
	"os/gpu_queue.cpp" 
	$<${SCIMITAR_WINDOWS}:${CMAKE_CURRENT_SOURCE_DIR}/os/window_win32.h>
	$<${SCIMITAR_WINDOWS}:${CMAKE_CURRENT_SOURCE_DIR}/os/window_win32.cpp>

	"input/input.cpp" 
	"input/input.h" 
	"input/keyboard.h" 
	"input/keyboard.cpp" 
	"input/mouse.h" 
	"input/mouse.cpp" 

	"math/helpers.h" 

	"renderer/renderer.h" 
	"renderer/renderer.cpp" 

	"util/algorithm.h"
	"util/biguint.h"
	"util/cast.h"
	"util/concepts.h"
	"util/debugger.h"
	"util/debugger.cpp"
	"util/endian.h" 
	
	"util/function.h" 
	"util/literals.h" 
	"util/memory.h"
	"util/spin_mutex.h" 
	"util/spin_mutex.cpp"
	"util/string_util.h" 
	"util/string_util.cpp"
	"util/threads.h" 
	"util/threads.cpp"
	"util/typemap.h" 
	"util/unique_resource.h"
	"util/codec/base_n.h"
	"util/codec/codepage_1252.h" 
	"util/codec/utf.h" 
	"util/codec/utf.cpp"
	"util/filesystem.h" 
	"util/filesystem.cpp"
	"util/flat_map.h" 
	"util/type_traits.h" 
	"util/exception.h"
)

# vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
find_package(gcem CONFIG REQUIRED)

find_path(RANG_INCLUDE_DIRS "rang.hpp")

# vulkan and shaderc are part of the Vulkan SDK 
find_package(Vulkan REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)

target_link_libraries(${SCIMITAR_LIB} 
PUBLIC
	Vulkan::Vulkan
	nlohmann_json
	nlohmann_json::nlohmann_json
	unofficial::vulkan-memory-allocator::vulkan-memory-allocator

PRIVATE
	gcem	
	unofficial::concurrentqueue::concurrentqueue	
	${SHADERC_LIB}
)

target_include_directories(${SCIMITAR_LIB} PRIVATE
	${Vulkan_INCLUDE_DIRS}
	${RANG_INCLUDE_DIRS}
)

