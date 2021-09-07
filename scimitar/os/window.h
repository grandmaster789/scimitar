#pragma once

#include "render_surface.h"
#include <memory>

namespace scimitar::os {
	class Window {
	public:
		Window() = default;
		Window(
			const char* title,
			int         width  = 1280,
			int         height = 720
		);
		~Window();

		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&& w) noexcept;
		Window& operator = (Window&& w) noexcept;

		virtual void init();
		virtual void shutdown();

		bool should_close() const noexcept;

	private:
		int   m_Width      = 1280;
		int   m_Height     = 720;
		bool  m_Fullscreen = false;
		float m_Dpi        = 72.0;


	};
}