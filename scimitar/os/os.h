#pragma once

#include <memory>
#include <vector>

#include "../dependencies.h"
#include "../core/system.h"
#include "render_device.h"

namespace scimitar::os {
	class Window;
}

namespace scimitar {
	class OS :
		public core::System
	{
	public:
		using RenderDevice = os::RenderDevice;
		using Window       = os::Window;

		OS();

		void init()     override; 
		void update()   override; // Platform-specific OS message pump
		void shutdown() override;

		const vk::Instance& get_vk_instance() const noexcept;

		const vk::PhysicalDeviceFeatures& get_vk_required_physical_features() const noexcept;
		const vk::PhysicalDeviceLimits&   get_vk_required_physical_limits()   const noexcept;

		void close(Window* w);

	private:
		void init_vulkan();

		static VkBool32 debug_callback(
			      VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			      VkDebugUtilsMessageTypeFlagsEXT        type,
			const VkDebugUtilsMessengerCallbackDataEXT*  data,
			void*                                        userdata
		);

		vk::UniqueInstance               m_VkInstance;
		vk::DispatchLoaderDynamic        m_VkLoader;
		vk::UniqueDebugUtilsMessengerEXT m_VkDebugMessager;

		// requirements
		std::vector<const char*>   m_RequiredInstanceExtensions;
		std::vector<const char*>   m_RequiredInstanceLayers;
		vk::PhysicalDeviceFeatures m_RequiredDeviceFeatures;
		vk::PhysicalDeviceLimits   m_RequiredDeviceLimits;
		
		std::vector<RenderDevice> m_RenderDevices;

		std::vector<std::unique_ptr<Window>> m_Windows;
	};
}