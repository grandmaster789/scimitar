#pragma once

#include <memory>
#include <vector>

#include "../dependencies.h"

#include "../core/system.h"
#include "window.h"
#include "render_device.h"

namespace scimitar {
	class OS :
		public core::System
	{
	public:
		OS();

		void init()     override; // by default, create a window
		void update()   override; // by default, stop the engine when all windows are closed
		void shutdown() override;

		os::Window* create_window(const std::string& title, int width, int height);

		const vk::Instance& get_vk_instance() const noexcept;

	private:
		using RenderDevice = os::RenderDevice;
		using Window       = os::Window;

		using WindowPtr = std::unique_ptr<Window>;
		
		std::vector<WindowPtr> m_Windows;

		struct WindowSettings {
			int  m_Width      = 1280;
			int  m_Height     = 720;
			bool m_Fullscreen = false;
		} m_WindowSettings;

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