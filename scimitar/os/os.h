#pragma once

#include <memory>
#include <vector>

#include "../dependencies.h"

#include "../core/system.h"
#include "render_device.h"

namespace scimitar {
	class OS :
		public core::System
	{
	public:
		OS();

		void init()     override; 
		void update()   override; // Platform-specific OS message pump
		void shutdown() override;

		const vk::Instance& get_vk_instance() const noexcept;

		const vk::PhysicalDeviceFeatures& get_vk_required_physical_features() const noexcept;
		const vk::PhysicalDeviceLimits&   get_vk_required_physical_limits()   const noexcept;

	private:
		using RenderDevice = os::RenderDevice;
		
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

		std::vector<std::unique_ptr<RenderDevice>> m_RenderDevices;
	};
}