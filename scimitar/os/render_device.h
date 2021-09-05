#pragma once

#include "../dependencies.h"

#include "../util/biguint.h"

#include "render_surface.h"
#include "gpu_queue.h"

namespace scimitar { class OS; }

namespace scimitar::os {
	class RenderDevice {
	public:
		RenderDevice(OS* os, vk::PhysicalDevice physical);
		~RenderDevice();

		// no-copy, no-move
		RenderDevice             (const RenderDevice&) = delete;
		RenderDevice& operator = (const RenderDevice&) = delete;
		RenderDevice             (RenderDevice&&)      = delete;
		RenderDevice& operator = (RenderDevice&&)      = delete;

		const GPU_Queue& getGraphicsQueue() const;
		const GPU_Queue& getGraphicsQueue(const RenderSurface& surface) const;
		const GPU_Queue& getPresentQueue (const RenderSurface& surface) const;

		vk::SurfaceFormatKHR getSurfaceFormat(const RenderSurface& surface, int* score = nullptr) const noexcept;

	private:
		std::string m_DeviceName = "[Unknown device]";

		vk::PhysicalDeviceType       m_DeviceType = vk::PhysicalDeviceType::eOther;
		vk::PhysicalDeviceProperties m_DeviceProperties;
		vk::PhysicalDevice           m_PhysicalDevice;
		vk::Device                   m_Device;    // logical device interface
		VmaAllocator                 m_Allocator; // allocates memory on the graphics card

		vk::PhysicalDeviceFeatures m_RequiredFeatures;
		std::vector<const char*>   m_RequiredExtensions;

		bool                m_SupportsLazyTransientImages = false;
		vk::ImageUsageFlags m_TransientImageUsageFlags    = vk::ImageUsageFlags{};
		VmaMemoryUsage      m_LazyMemoryUsage             = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;

		std::vector<GPU_Queue> m_Queues;
	};
}