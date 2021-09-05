#pragma once

// Using a single header here to control symbol definition order (sadly something that needs to be considered)
// The alternative is to provide these flags via cmake, but I prefer this

#include "preprocessor.h"

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
#else
	#error "Please specify which vulkan platform to use...""
#endif

// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html
// current version: v1.2.190 
#include <vulkan/vulkan.hpp>

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	#include <vulkan/vulkan_win32.h>
#else
	#error "Please included the appropriate vulkan platform header..."
#endif

// https://gpuopen.com/vulkan-memory-allocator/
#include <vk_mem_alloc.h> 

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>

// https://gcem.readthedocs.io/en/latest/
#include <gcem.hpp> // constexpr math functions (std::sqrt is not constexpr...)
