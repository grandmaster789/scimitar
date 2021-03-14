#include "os.h"
#include "../util/algorithm.h"
#include "../core/engine.h"

namespace scimitar {
	OS::OS():
		System("OS")
	{
		add_dependency("Input"); // because we create a window during initialization, and that window needs to register its inputs somewhere

		register_setting("window_width",      &m_WindowSettings.m_Width);
		register_setting("window_height",     &m_WindowSettings.m_Height);
		register_setting("window_fullscreen", &m_WindowSettings.m_Fullscreen);
	}

	void OS::init() {
		System::init();

		createWindow(
			"Scimitar", 
			m_WindowSettings.m_Width,
			m_WindowSettings.m_Height
		);
		
		{
			vk::ApplicationInfo ai;
			ai.setApiVersion        (VK_API_VERSION_1_2);
			ai.setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));
			ai.setEngineVersion     (VK_MAKE_VERSION(0, 1, 0));
			ai.setPApplicationName  ("Scimitar application");
			ai.setPEngineName       ("Scimitar engine");

			uint32_t     num_extensions  = 0;

			std::vector<const char*> requiredInstanceExtensions;

			//~~

			vk::InstanceCreateInfo ici;
			ici.setPApplicationInfo(&ai);
			ici.setEnabledExtensionCount(num_extensions);
			ici.setPEnabledExtensionNames(requiredInstanceExtensions);
			ici.setEnabledLayerCount(0);

			m_VkInstance = vk::createInstanceUnique(ici);
		}
	}

	void OS::update() {
		auto local_copy = util::create_non_owning_copy(m_Windows);

		for (auto* win : local_copy) {
			if (win->should_close())
				std::erase_if(m_Windows, [win](const auto& ptr) { return ptr.get() == win; });
		}

		if (m_Windows.empty())
			m_Engine->stop();
	}

	void OS::shutdown() {
		System::shutdown();
	}

	os::Window* OS::createWindow(
		const std::string& title, 
		int                width, 
		int                height
	) {
		m_Windows.push_back(
			std::make_unique<os::Window>(
				title.c_str(), 
				width, 
				height
			)
		);

		return m_Windows.back().get();
	}
}