#include "window.h"
#include "../dependencies.h"
#include "../util/debugger.h"
#include "../util/string_util.h"
#include "../util/codec/utf.h"

#include <cassert>

namespace {
	static const char* s_WindowClassName       = nullptr;
	static WNDCLASS    s_WindowClass           = {};
	static bool        s_WindowClassRegistered = false;
	static bool        s_WindowOpened          = false;

	static LRESULT CALLBACK window_message_handler(
		HWND   window_handle,
		UINT   message,
		WPARAM wParam,
		LPARAM lParam
	) noexcept {
		auto userdata = GetWindowLongPtr(window_handle, 0);
		if (!userdata)
			return DefWindowProc(window_handle, message, wParam, lParam);

		auto* window = std::launder(std::bit_cast<scimitar::gui::Window*>(userdata));

		LRESULT proc_result = window->handle_os_message(message, wParam, lParam);

		if (message == WM_DESTROY)
			window->request_close();

		if (proc_result == -1)
			return DefWindowProc(window_handle, message, wParam, lParam);

		return proc_result;
	}

	static void create_window_class() {
		if (!s_WindowClassRegistered) {
			s_WindowClassName = "Scimitar WindowClass";

			// https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassw
			// https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles

			s_WindowClass               = {};
			s_WindowClass.style         = CS_DBLCLKS; // send double click messages when user double clicks in the window area
			s_WindowClass.lpfnWndProc   = window_message_handler;
			s_WindowClass.cbClsExtra    = 0;             // extra bytes for the class structure
			s_WindowClass.cbWndExtra    = sizeof(void*); // extra bytes for the window structure (we're going to put a pointer in these bytes)
			s_WindowClass.hInstance     = GetModuleHandle(NULL);
			s_WindowClass.hIcon         = nullptr; // LoadIcon(NULL, IDI_APPLICATION);
			s_WindowClass.hCursor       = nullptr; // LoadCursor(NULL, IDC_ARROW);
			s_WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			s_WindowClass.lpszMenuName  = nullptr;
			s_WindowClass.lpszClassName = s_WindowClassName;
			
			RegisterClass(&s_WindowClass); // NOTE seems like unregistering is not required?

			s_WindowClassRegistered = true;
		}
	}

	std::vector<DISPLAY_DEVICE> enumerateDisplayDevices() {
		std::vector<DISPLAY_DEVICE> result;

		for (int i = 0;; ++i) {
			DISPLAY_DEVICE dd = {};
			dd.cb = sizeof(dd);

			// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-enumdisplaydevicesa
			if (!EnumDisplayDevices(NULL, i, &dd, 0))
				break;  // stop if we couldn't find more devices

			if (dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
				continue;  // skip over mirroring (pseudo) devices

			result.push_back(dd);
		}

		return result;
	}

	DEVMODE getCurrentDisplayMode(DISPLAY_DEVICE dd) {
		DEVMODE mode = {};

		mode.dmSize = sizeof(mode);
		
		if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &mode))
			throw std::runtime_error("Failed to query current display mode");

		return mode;
	}
}

namespace scimitar::gui {
	void Window::create_window(
		const std::string& title,
		int width,
		int height
	) {
		// TODO ensure that this is only done from the GUI thread
		create_window_class();
		
		// The recommended value for dpi-awareness; needs to be set before opening a window
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		DWORD style   = 0; // https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles
		DWORD exStyle = 0; // https://docs.microsoft.com/en-us/windows/desktop/winmsg/extended-window-styles

		// try and center the first window
		RECT rect;
		
		auto displayDevices = enumerateDisplayDevices();                // aka display monitors
		auto deviceMode     = getCurrentDisplayMode(displayDevices[0]); // just pick the first one I guess

		{
			// center the rect
			int device_x = deviceMode.dmPosition.x;
			int device_y = deviceMode.dmPosition.y;
			int device_w = deviceMode.dmPelsWidth;
			int device_h = deviceMode.dmPelsHeight;

			rect.left   = device_x  + (device_w - width) / 2;
			rect.top    = device_y  + (device_h - height) / 2;
			rect.right  = rect.left + width;
			rect.bottom = rect.top  + height;

			AdjustWindowRectEx(
				&rect, 
				style, 
				FALSE, // bMenu
				exStyle
			);
		}

		m_Handle = CreateWindowEx(
			exStyle,
			s_WindowClassName,
			title.data(),
			style,
			rect.left,
			rect.top,
			rect.right - rect.left, // width
			rect.bottom - rect.top, // height
			nullptr,                // parent window
			nullptr,                // menu
			GetModuleHandle(NULL),  // hInstance
			nullptr                 // additional parameters
		);

		if (m_Handle) {
			SetWindowLongPtr(m_Handle, 0, (LONG_PTR)this); // store the associated object pointer

			// extend the drawable area
			// https://docs.microsoft.com/en-us/windows/win32/api/dwmapi/nf-dwmapi-dwmextendframeintoclientarea
			MARGINS margins{ 0, 0, 0, 1 };
			DwmExtendFrameIntoClientArea(m_Handle, &margins);

			// force WM_NCCALCSIZE message
			// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
			SetWindowPos(
				m_Handle,
				HWND_TOP,				// hwndInsertAfter (z-order)
				0,						// left in client coordinates
				0,						// top  in client coordinates
				0,                      // window width in pixels
				0,					    // window height in pixels
				SWP_NOOWNERZORDER |     // do not change the owner position in Z order
				SWP_NOZORDER      |     // retain the current Z order  (ignore insert_after parameter)
				SWP_NOMOVE        |     // retain the current position (ignore left/top parameters)
				SWP_NOSIZE        |     // retain the current size     (ignore width/height parameters)
				SWP_FRAMECHANGED        // apply new frame style, send WM_NCCALCSIZE to the window
			);

			// first window should be displayed by default
			if (!s_WindowOpened) {
				ShowWindow(m_Handle, SW_SHOW);
				SetForegroundWindow(m_Handle);

				s_WindowOpened = true;
			}

			m_Dpi = GetDpiForWindow(m_Handle);
			if (m_Dpi == 0)
				throw std::runtime_error("Failed to retrieve DPI value for window");
			
			// create a vulkan surface for this window
		}
		else
			throw std::runtime_error("Failed to create window");
	}

	int Window::handle_os_message(
		unsigned int message,
		uint64_t     wParam,
		int64_t      lParam
	) noexcept {


		return -1;
	}
}
