#include "window.h"

#include <utility>

namespace scimitar::os {
	// https://www.glfw.org/docs/latest/group__window.html
	
	Window::Window(
		const char* title, 
		int width, 
		int height
	):
		m_Width(width),
		m_Height(height)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // only use it for window management, not for openGL
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // disallow window resizing for now

		m_Handle = glfwCreateWindow(
			m_Width,
			m_Height,
			title,
			nullptr, // window title
			nullptr  // shared context
		);
	}

	Window::~Window() {
		if (m_Handle)
			glfwDestroyWindow(m_Handle);
	}

	Window::Window(Window&& w) noexcept {
		std::swap(m_Handle,     w.m_Handle);
		std::swap(m_Width,      w.m_Width);
		std::swap(m_Height,     w.m_Height);
		std::swap(m_Fullscreen, w.m_Fullscreen);
	}

	Window& Window::operator = (Window&& w) noexcept {
		std::swap(m_Handle,     w.m_Handle);
		std::swap(m_Width,      w.m_Width);
		std::swap(m_Height,     w.m_Height);
		std::swap(m_Fullscreen, w.m_Fullscreen);

		return *this;
	}

	bool Window::should_close() const {
		return (glfwWindowShouldClose(m_Handle) == GLFW_TRUE);
	}

	GLFWwindow* Window::handle() const {
		return m_Handle;
	}
}