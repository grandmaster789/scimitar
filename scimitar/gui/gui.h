#pragma once

#include "../core/system.h"
#include "../util/threads.h"

#include <vector>
#include <string>

namespace scimitar {
	namespace gui {
		class Window;
	}

	class Gui:
		public core::System
	{
	public:
		using Window = gui::Window;

		Gui();

		void init()     override;
		void update()   override;
		void shutdown() override;

		bool is_gui_thread() const noexcept;

		Window* create_window(
			const std::string& title, 
			int                width, 
			int                height
		);

	private:
		struct WindowSettings {
			int  m_Width      = 1280;
			int  m_Height     = 720;
			bool m_Fullscreen = false;
		} m_MainWindowSettings;

		util::ThreadID m_ThreadID; // hm

		std::vector<std::unique_ptr<Window>> m_Windows;
	};
}