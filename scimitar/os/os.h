#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../core/system.h"
#include "window.h"

namespace scimitar {
	class OS :
		public core::System
	{
	public:
		OS();

		void init()     override; // by default, create a window
		void update()   override; // by default, stop the engine when all windows are closed
		void shutdown() override;

		os::Window* createWindow(const std::string& title, int width, int height);

	private:
		using WindowPtr = std::unique_ptr<os::Window>;
		std::vector<WindowPtr> m_Windows;

		struct WindowSettings {
			int  m_Width      = 1280;
			int  m_Height     = 720;
			bool m_Fullscreen = false;
		} m_WindowSettings;

		vk::UniqueInstance m_VkInstance;
	};
}