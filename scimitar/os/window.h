#pragma once

#include <GLFW/glfw3.h>

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

		bool should_close() const;

		GLFWwindow* handle() const;

	private:
		GLFWwindow* m_Handle = nullptr;

		int  m_Width      = 1280;
		int  m_Height     = 720;
		bool m_Fullscreen = false;
	};
}