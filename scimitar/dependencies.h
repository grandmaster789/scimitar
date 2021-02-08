#pragma once

#include "preprocessor.h"

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
#else
	#error "Please specify which vulkan platform to use...""
#endif

// https://www.glfw.org/docs/latest/
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html
// current version: v1.2.169 
#include <vulkan/vulkan.hpp>

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>