#include "window.h"
#include "os.h"

#include "../core/logger.h"
#include "../core/engine.h"
#include "../input/input.h"
#include "../dependencies.h"

#include <cassert>

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
		auto userdata = GetWindowLongPtr(window, GWLP_USERDATA);

		if (userdata == 0)
			return DefWindowProc(window, msg, wp, lp);

		auto* win = (scimitar::os::Window*)userdata;

		auto proc_result = win->win_proc(window, msg, wp, lp);

		if (proc_result == -1)
			return DefWindowProc(window, msg, wp, lp);

		return proc_result;
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
			m_WindowClass.style         = CS_DBLCLKS; // enable double-click window message
			m_WindowClass.lpfnWndProc   = defer_win_proc;
			m_WindowClass.cbClsExtra    = 0; // extra bytes for the WindowClass
			m_WindowClass.cbClsExtra    = 0; // extra bytes for the window itself
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
		// Set Dpi awareness before actually opening any window
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		auto display_monitors = enumerate_display_devices();

		if (display_monitors.empty())
			throw std::runtime_error("No display devices are available");

		if (display_monitors.size() < display_device_idx) {
			gLogWarning << "Display " << display_device_idx << " unavailable, falling back to primary display";
			display_device_idx = 0;
		}

		auto device_mode = get_current_displaymode(display_monitors[display_device_idx]);

		const auto& wc = ScimitarWindowClass::instance().get();

		DWORD style   = 0;
		DWORD exStyle = 0;

		if (windowed) {
			style =
				WS_CLIPSIBLINGS |
				WS_CLIPCHILDREN |
				WS_SYSMENU |
				WS_MINIMIZEBOX;

			exStyle = WS_EX_APPWINDOW;
		}
		else {
			style =
				WS_CLIPSIBLINGS |
				WS_CLIPCHILDREN |
				WS_POPUP;

			exStyle = 
				WS_EX_APPWINDOW | 
				WS_EX_TOPMOST;
		}

		// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		m_NativeHandle = CreateWindowEx(
			exStyle,
			wc.lpszClassName,
			title.c_str(),
			style,
			CW_USEDEFAULT,			// x
			CW_USEDEFAULT,			// y
			CW_USEDEFAULT,          // width
			CW_USEDEFAULT,          // height
			nullptr,                // parent window
			nullptr,                // menu
			GetModuleHandle(NULL),  // hInstance
			this                    // LPVOID lpParam (additional parameters)
		);

		if (m_NativeHandle) {
			// associate this object with the native window
			SetWindowLongPtr(m_NativeHandle, GWLP_USERDATA, (LONG_PTR)this);

			ShowWindow(m_NativeHandle, SW_SHOW);
		}
		else
			throw std::runtime_error("Failed to create window");

		auto* input_system = core::Engine::instance().get<Input>();
		
		m_Keyboard = std::make_unique<Keyboard>(input_system);
		m_Mouse    = std::make_unique<Mouse>(input_system);

		// create the vulkan surface
		/* 
		{
			vk::Win32SurfaceCreateInfoKHR info;

			info
				.setHinstance(GetModuleHandle(NULL))
				.setHwnd(m_NativeHandle);

			m_Surface = m_Owner->get_vk_instance().createWin32SurfaceKHRUnique(info);

			if (!m_Surface)
				throw std::runtime_error("Failed to create vulkan surface");
		}
		*/
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

	/*
	vk::SurfaceKHR Window::getSurface() const {
		return m_Surface.get();
	}
	*/

	LRESULT CALLBACK Window::win_proc(
		HWND   window,
		UINT   msg,
		WPARAM wp,
		LPARAM lp
	) {
		assert(window == m_NativeHandle);

		switch (msg) {
		case WM_DESTROY:
			// remove the associated object pointer to prevent further cascades
			SetWindowLongPtr(window, GWLP_USERDATA, NULL);

			m_Owner->close(this);

			break;

		case WM_CREATE: {
				auto* info = (CREATESTRUCT*)lp;
				RECT rect;

				rect.left   = info->x;
				rect.top    = info->y;
				rect.right  = info->x + info->cx;
				rect.bottom = info->y + info->cy;

				// should update layout here
			}
			break;

		case WM_PAINT: {

			}
			break;
		}

		return -1;
	}
}