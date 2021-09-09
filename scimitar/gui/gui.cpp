#include "gui.h"

#include "window.h"

#include "../core/engine.h"
#include "../util/algorithm.h"

namespace scimitar {
	Gui::Gui() :
		System("Gui")
	{
		// depends on OS for fetching GPU devices and Input to register input devices with
		add_dependency("OS");
		add_dependency("Input");

		register_setting("MainWindowWidth",      &m_MainWindowSettings.m_Width);
		register_setting("MainWindowHeight",     &m_MainWindowSettings.m_Height);
		register_setting("MainWindowFullscreen", &m_MainWindowSettings.m_Fullscreen);

		// engine will do the configuration etc in its main thread, then init/update/operate/shutdown in a dedicated one
		m_DedicatedThread = true; 
	}

	void Gui::init() {
		System::init();

		create_window(
			"Main window", 
			m_MainWindowSettings.m_Width, 
			m_MainWindowSettings.m_Height
		);
	}

	void Gui::update() {
		auto local_copy = util::create_non_owning_copy(m_Windows);

		for (auto* win : local_copy) {
			if (win->should_close())
				std::erase_if(m_Windows, [win](const auto& ptr) { return ptr.get() == win; });
		}

		// if all windows have closed, shut down the entire application
		if (m_Windows.empty())
			m_Engine->stop();
	}

	void Gui::shutdown() {
		System::shutdown();
	}

	bool Gui::is_gui_thread() const noexcept {
		return (util::get_current_threadID() == m_ThreadID);
	}

	gui::Window* Gui::create_window(
		const std::string& title,
		int                width,
		int                height
	) {
		m_Windows.push_back(
			std::make_unique<Window>(
				title,
				width,
				height
			)
		);

		return m_Windows.back().get();
	}
}