#include "window.h"

namespace scimitar::gui {
	Window::Window(
		const std::string& title,
		int                width,
		int                height
	) noexcept {
	}

	bool Window::should_close() const noexcept {
		return false;
	}
}