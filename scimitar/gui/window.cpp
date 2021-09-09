#include "window.h"

namespace scimitar::gui {
	Window::Window(
		const std::string& title,
		int                width,
		int                height
	) noexcept {
		create_window(title, width, height);
	}

	bool Window::should_close() const noexcept {
		return m_ShouldClose;
	}

	void Window::request_close() noexcept {
		m_ShouldClose = true;
	}
}