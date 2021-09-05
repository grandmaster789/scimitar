#include "render_device.h"
#include "os.h"
#include "../util/debugger.h"

namespace scimitar::os {
	RenderDevice::RenderDevice(OS* os, vk::PhysicalDevice physical):
		m_PhysicalDevice(physical)
	{
		auto props2        = physical.getProperties2();
		m_DeviceProperties = props2.properties;
		m_DeviceName       = std::string(&m_DeviceProperties.deviceName[0]); // convert from vk::ArrayWrapper<char, 256>
		
		// initialize the logical device
		std::array<float, 1> default_queue_priority = { 1.0f };
		uint32_t             queue_family_index     = 0;

		std::vector<vk::DeviceQueueCreateInfo> dqci;
		
		for (const auto& properties : m_PhysicalDevice.getQueueFamilyProperties()) {
			uint32_t num_queues = 1;

			vk::DeviceQueueCreateInfo info(
				vk::DeviceQueueCreateFlags(),
				queue_family_index++,
				num_queues,
				default_queue_priority.data()
			);

			dqci.push_back(std::move(info));
		}

		m_RequiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);

		vk::DeviceCreateInfo dci(
			vk::DeviceCreateFlags(),
			static_cast<uint32_t>(dqci.size()),
			dqci.data(),
			0,         // enabledLayerCount,
			nullptr,   // ppEnabledLayerNames,
			static_cast<uint32_t>(m_RequiredExtensions.size()),
			m_RequiredExtensions.data(),
			&m_RequiredFeatures
		);

		m_Device = m_PhysicalDevice.createDevice(dci);

		// initialize the memory allocator
		VmaAllocatorCreateInfo vaci = {};
		vaci.physicalDevice = m_PhysicalDevice;
		vaci.device         = m_Device;
		vaci.instance       = os->get_vk_instance();
		vmaCreateAllocator(&vaci, &m_Allocator);

		// figure out if lazy transient images are supported
		VmaAllocationCreateInfo lazy_alloc_info = {};
		lazy_alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
		uint32_t type_index = 0;

		if (vmaFindMemoryTypeIndex(m_Allocator, 0, &lazy_alloc_info, &type_index) != VkResult::VK_ERROR_FEATURE_NOT_PRESENT) {
			m_SupportsLazyTransientImages = true;
			m_TransientImageUsageFlags   &= vk::ImageUsageFlagBits::eTransientAttachment;
			m_LazyMemoryUsage             = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
		}

		// initialize the queues, create command pools for everything
		auto family_prop_list = m_PhysicalDevice.getQueueFamilyProperties();

		for (const auto& info : dqci) {
			uint32_t    queue_family_idx = info.queueFamilyIndex;
			const auto& props            = family_prop_list[queue_family_index];
			const auto  flags            = props.queueFlags;

			for (uint32_t queue_idx = 0; queue_idx != info.queueCount; ++queue_idx) {
				auto vk_queue = m_Device.getQueue(queue_family_idx, queue_idx);

				vk::CommandPoolCreateInfo cpci(
					vk::CommandPoolCreateFlagBits::eTransient |
					vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
					queue_family_idx
				);

				auto commandpool = m_Device.createCommandPoolUnique(cpci);

				GPU_Queue queue(
					queue_family_idx,
					queue_idx,
					flags,
					std::move(vk_queue),
					std::move(commandpool)
				);

				m_Queues.push_back(std::move(queue));
			}
		}

		//init_quad_idx_buffer();

		// create pipelines for the various renderers
	}

	RenderDevice::~RenderDevice() {
		try {

		}
		catch (std::exception& e) {
			debugger_abort(e.what());
		}
	}

	const GPU_Queue& RenderDevice::getGraphicsQueue() const {
		for (const auto& queue : m_Queues)
			if (queue.is_graphics())
				return queue;

		throw std::runtime_error("No graphics queue availaible at all");
	}

	const GPU_Queue& RenderDevice::getGraphicsQueue(const RenderSurface& surface) const {
		const GPU_Queue* fallback = nullptr;
		auto vk_surface = surface.get_vk_surface();

		// try to find something that can present to the given surface
		for (const auto& queue : m_Queues) {
			if (queue.is_graphics()) {
				if (m_PhysicalDevice.getSurfaceSupportKHR(
					queue.get_family_queue_idx(),
					vk_surface
				))
					return queue;

				// keep the first graphics queue as a fallback jic
				if (!fallback)
					fallback = &queue;
			}
		}

		if (fallback == nullptr)
			throw std::runtime_error("No graphics queue available at all");

		return *fallback;
	}

	const GPU_Queue& RenderDevice::getPresentQueue(const RenderSurface& surface) const {
		auto vk_surface = surface.get_vk_surface();

		for (const auto& queue : m_Queues) {
			if (queue.is_graphics()) {
				if (m_PhysicalDevice.getSurfaceSupportKHR(
					queue.get_family_queue_idx(),
					vk_surface
				))
					return queue;
			}
		}

		throw std::runtime_error("None of the queues may present");
	}

	vk::SurfaceFormatKHR RenderDevice::getSurfaceFormat(
		const RenderSurface& surface, 
		int* score 
	) const noexcept {

	}
}