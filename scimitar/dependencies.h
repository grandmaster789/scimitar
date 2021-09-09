#pragma once

// Using a single header here to control symbol definition order (sadly something that needs to be considered)
// The alternative is to provide these flags via cmake, but I prefer this

#include "preprocessor.h"

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
#else
	#error "Please specify which vulkan platform to use...""
#endif

#if SCIMITAR_COMPILER == SCIMITAR_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable: 26495) // always initialize member variables
	#pragma warning(disable: 26812) // always prefer scoped enums
	#pragma warning(disable: 26451) // potential arithmetic overflow
	#pragma warning(disable: 28251) // Inconsistent annotation
#endif

// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html
// current version: v1.2.190 
#include <vulkan/vulkan.hpp>

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	#include <vulkan/vulkan_win32.h>
#else
	#error "Please included the appropriate vulkan platform header..."
#endif

#if SCIMITAR_COMPILER == SCIMITAR_COMPILER_MSVC
	#pragma warning(pop)
#endif

// https://gpuopen.com/vulkan-memory-allocator/
#include <vk_mem_alloc.h>

#if SCIMITAR_COMPILER == SCIMITAR_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable: 26819) // unannotated fallthrough statements in switch
#endif

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>

#if SCIMITAR_COMPILER == SCIMITAR_COMPILER_MSVC
	#pragma warning(pop)
#endif

// https://gcem.readthedocs.io/en/latest/
#include <gcem.hpp> // constexpr math functions (std::sqrt is not constexpr...)
