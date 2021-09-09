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
		void request_close() noexcept;      // upon next update, the window will be closed

		// platform-specfic OS message handling
#if SCIMITAR_PLATFORM ==SCIMITAR_PLATFORM_WINDOWS
		int handle_os_message(
			unsigned int message,
			uint64_t     wParam,
			int64_t      lParam
		) noexcept;
#else
	#error "Not yet implemented"
#endif

	private:
		bool m_ShouldClose = false;

		geometry::AxisAlignedRectangle m_ScreenRect;

		std::unique_ptr<os::RenderSurface> m_Surface;

		// platform-specfic handles, helper methods etc
#if SCIMITAR_PLATFORM ==SCIMITAR_PLATFORM_WINDOWS
		void create_window(
			const std::string& title,
			int                width,
			int                height
		);

		HWND m_Handle;
		UINT m_Dpi;
#else
	#error "Not yet implemented"
#endif
	};
}