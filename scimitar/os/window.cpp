#include "window.h"

#include <utility>

namespace scimitar::os {
	Window::Window(
		const char* title, 
		int width, 
		int height
	):
		m_Width(width),
		m_Height(height)
	{
		(void)title;
	}

	Window::~Window() {
		
	}

	Window::Window(Window&& w) noexcept {
		
	}

	Window& Window::operator = (Window&& w) noexcept {
		return *this;
	}

	bool Window::should_close() const noexcept {
		return false;
	}

	void Window::init() {

	}

	void Window::shutdown() {

	}
}