#pragma once

#include <string>
#include <memory>

#include "../geometry/aa_rectangle.h"
#include "../os/render_surface.h"

namespace scimitar::gui {
	class Window {
	public:
		Window(
			const std::string& title,
			int                width,
			int                height
		) noexcept;

		// no-copy, default-move
		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&&)      = default;
		Window& operator = (Window&&)      = default;

		bool should_close() const noexcept; // doesn't close immediately but during the next Gui::update()

		

	private:
		geometry::AxisAlignedRectangle m_ScreenRect;

		std::unique_ptr<os::RenderSurface> m_Surface;
	};
}