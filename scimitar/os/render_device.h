#pragma once

#include "../dependencies.h"

#include "../util/biguint.h"

#include "render_surface.h"
#include "gpu_queue.h"

namespace scimitar { class OS; }

namespace scimitar::os {
	/*
	*	Combine a physical device with a logical one, and provide queues to interact with it
	*   (focused on graphics atm)
	*/
	class RenderDevice {
	public:
		RenderDevice(OS* os, vk::PhysicalDevice physical);
		~RenderDevice();

		// no-copy, no-move
		RenderDevice             (const RenderDevice&) = delete;
		RenderDevice& operator = (const RenderDevice&) = delete;
		RenderDevice             (RenderDevice&&)      = delete;
		RenderDevice& operator = (RenderDevice&&)      = delete;

		vk::Device get_device() const;

		const GPU_Queue& get_graphics_queue() const;
		const GPU_Queue& get_graphics_queue(const RenderSurface& surface) const;
		const GPU_Queue& get_present_queue (const RenderSurface& surface) const;

		int score(const RenderSurface& surface) const noexcept; // yield a 'suitability score', higher should provide better performance and/or more features

		vk::SurfaceFormatKHR get_surface_format(const RenderSurface& surface, int* score = nullptr) const noexcept;
		vk::PresentModeKHR   get_present_mode  (const RenderSurface& surface, int* score = nullptr) const noexcept;

	private:
		std::string m_DeviceName = "[Unknown device]";

		vk::PhysicalDeviceType       m_DeviceType = vk::PhysicalDeviceType::eOther;
		vk::PhysicalDeviceProperties m_DeviceProperties;
		vk::PhysicalDevice           m_PhysicalDevice;
		vk::UniqueDevice             m_Device;    // logical device interface
		VmaAllocator                 m_Allocator; // allocates memory on the graphics card

		vk::PhysicalDeviceFeatures m_RequiredFeatures;
		vk::PhysicalDeviceLimits   m_RequiredLimits;
		std::vector<const char*>   m_RequiredExtensions;

		bool                m_SupportsLazyTransientImages = false;
		vk::ImageUsageFlags m_TransientImageUsageFlags    = vk::ImageUsageFlags{};
		VmaMemoryUsage      m_LazyMemoryUsage             = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;

		std::vector<GPU_Queue> m_Queues;
	};
}