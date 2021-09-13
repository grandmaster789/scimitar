#include "window.h"
#include <cassert>
#include "../core/logger.h"
#include "os.h"

/*
*	Very much win32 api code in here
*/

namespace {
	LRESULT CALLBACK defer_win_proc(
		HWND   window,
		UINT   msg,
		WPARAM wp,
		LPARAM lp
	) {
		LONG_PTR ptr = GetWindowLongPtr(window, 0); // fetch associated userdata

		if (ptr == NULL)
			return DefWindowProc(window, msg, wp, lp);
		else
			return std::bit_cast<scimitar::os::Window*>(ptr)->win_proc(window, msg, wp, lp);
	}

	struct ScimitarWindowClass {
		static ScimitarWindowClass& instance() {
			static ScimitarWindowClass x;
			return x;
		}

		const WNDCLASSEX& get() const {
			return m_WindowClass;
		}

	private:
		ScimitarWindowClass() {
			m_WindowClass.cbSize        = sizeof(m_WindowClass);
			m_WindowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			m_WindowClass.lpfnWndProc   = defer_win_proc;
			m_WindowClass.cbClsExtra    = 0;             // extra bytes for the WindowClass
			m_WindowClass.cbClsExtra    = sizeof(void*); // extra bytes for the window itself (we're going to put a pointer to the associated object there)
			m_WindowClass.hInstance     = GetModuleHandle(NULL);
			m_WindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			m_WindowClass.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);
			m_WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
			m_WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			m_WindowClass.lpszMenuName  = nullptr;
			m_WindowClass.lpszClassName = "ScimitarWindowClass";

			if (RegisterClassEx(&m_WindowClass) == 0)
				throw std::runtime_error("Failed to register WindowClass");
		}

		~ScimitarWindowClass() {
			UnregisterClass(
				m_WindowClass.lpszClassName, 
				GetModuleHandle(NULL)
			);
		}

		WNDCLASSEX m_WindowClass = {};
	};

	std::vector<DISPLAY_DEVICE> enumerate_display_devices() {
		std::vector<DISPLAY_DEVICE> result;

		for (int i = 0;; ++i) {
			DISPLAY_DEVICE dd = {};
			dd.cb = sizeof(dd);

			// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-enumdisplaydevicesa
			if (!EnumDisplayDevices(NULL, i, &dd, 0))
				break; // stop if we coundn't find any more devices

			if (dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
				continue; // skip over mirroring (pseudo) devices

			result.push_back(dd);
		}

		return result;
	}

	DEVMODE get_current_displaymode(DISPLAY_DEVICE dd) {
		DEVMODE mode = {};

		mode.dmSize = sizeof(mode);
		mode.dmDriverExtra = 0;

		if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &mode))
			throw std::runtime_error("Failed to query current display mode");

		return mode;
	}
}

namespace scimitar::os {
	Window::Window(
		const std::string& title,
		uint32_t           width,
		uint32_t           height,
		bool               windowed,
		int                display_device_idx,
		OS*                owner
	):
		m_Owner(owner)
	{
		auto display_monitors = enumerate_display_devices();

		if (display_monitors.empty())
			throw std::runtime_error("No display devices are available");

		if (display_monitors.size() < display_device_idx) {
			gLogWarning << "Display " << display_device_idx << " unavailable, falling back to primary display";
			display_device_idx = 0;
		}

		auto device_mode = get_current_displaymode(display_monitors[display_device_idx]);

		RECT rect;

		DWORD style   = 0; // https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles
		DWORD exStyle = 0; // https://docs.microsoft.com/en-us/windows/desktop/winmsg/extended-window-styles

		int device_x = device_mode.dmPosition.x; // device mode position is a virtual desktop location
		int device_y = device_mode.dmPosition.y;
		int device_w = device_mode.dmPelsWidth;
		int device_h = device_mode.dmPelsHeight;

		if (windowed) {
			// center the rect
			rect.left   = device_x  + (device_w - width)  / 2;
			rect.top    = device_y  + (device_h - height) / 2;
			rect.right  = rect.left + width;
			rect.bottom = rect.top  + height;

			style   = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}
		else {
			// fill the entire device space
			// (NOTE this is a 'Fullscreen Windowed' approach, no finessing with the video mode here
			rect.left   = device_x;
			rect.right  = device_x + device_w;
			rect.top    = device_y;
			rect.bottom = device_y + device_h;

			style   = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			exStyle = WS_EX_APPWINDOW;
		}

		AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		const auto& wc = ScimitarWindowClass::instance().get();

		m_NativeHandle = CreateWindowEx(
			exStyle,
			wc.lpszClassName,
			title.c_str(),
			style,
			rect.left,
			rect.top,
			rect.right - rect.left, // width
			rect.bottom - rect.top, // height
			nullptr,                // parent window
			nullptr,                // menu
			GetModuleHandle(NULL),  // hInstance
			nullptr                 // LPVOID lpParam (additional parameters)
		);

		if (m_NativeHandle) {
			SetWindowLongPtr(m_NativeHandle, 0, (LONG_PTR)this);

			if (!s_MainWindow)
				s_MainWindow = m_NativeHandle;

			GetClientRect(m_NativeHandle, &rect);
			m_Width  = rect.right  - rect.left;
			m_Height = rect.bottom - rect.top;

			ShowWindow(m_NativeHandle, SW_SHOW);
			SetForegroundWindow(m_NativeHandle);
		}
		else
			throw std::runtime_error("Failed to create window");
	}

	Window::~Window() {
		if (m_NativeHandle)
			DestroyWindow(m_NativeHandle);
	}

	Window::Handle Window::get_native_handle() const {
		return m_NativeHandle;
	}

	bool Window::is_main_window() const {
		return m_NativeHandle == s_MainWindow;
	}

	const Window::Keyboard* Window::get_keyboard() const {
		return m_Keyboard.get();
	}
	
	const Window::Mouse* Window::get_mouse() const {
		return m_Mouse.get();
	}

	uint32_t Window::getWidth()  const {
		return m_Width;
	}

	uint32_t Window::getHeight() const {
		return m_Height;
	}

	vk::SurfaceKHR Window::getSurface() const {
		return m_Surface.get();
	}

	LRESULT CALLBACK Window::win_proc(
		HWND   window,
		UINT   msg,
		WPARAM wp,
		LPARAM lp
	) {
		assert(window == m_NativeHandle);

		switch (msg) {
		case WM_DESTROY:
			if (is_main_window())
				PostQuitMessage(0);
			break;

		case WM_CLOSE:
			m_Owner->close(this);
			break;
		}

		return DefWindowProc(window, msg, wp, lp);
	}
}