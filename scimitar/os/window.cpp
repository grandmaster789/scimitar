#include "window.h"
#include "os.h"

#include "../core/logger.h"
#include "../core/engine.h"
#include "../input/input.h"
#include "../dependencies.h"
#include "../util/flat_map.h"

#include <cassert>

/*
*	Very much win32 api code in here
*/

namespace {
	scimitar::util::FlatMap<WPARAM, scimitar::input::Keyboard::eKey> g_KeyMapping;

	void populate_keymapping() {
		using eKey = scimitar::input::Keyboard::eKey;

		// VK_ macros (virtual key codes) can be found in <winuser.h>
		// https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes

		// letters * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		g_KeyMapping.assign('A', eKey::a);
		g_KeyMapping.assign('B', eKey::b);
		g_KeyMapping.assign('C', eKey::c);
		g_KeyMapping.assign('D', eKey::d);

		g_KeyMapping.assign('E', eKey::e);
		g_KeyMapping.assign('F', eKey::f);
		g_KeyMapping.assign('G', eKey::g);
		g_KeyMapping.assign('H', eKey::h);

		g_KeyMapping.assign('I', eKey::i);
		g_KeyMapping.assign('J', eKey::j);
		g_KeyMapping.assign('K', eKey::k);
		g_KeyMapping.assign('L', eKey::l);

		g_KeyMapping.assign('M', eKey::m);
		g_KeyMapping.assign('N', eKey::n);
		g_KeyMapping.assign('O', eKey::o);
		g_KeyMapping.assign('P', eKey::p);

		g_KeyMapping.assign('Q', eKey::q);
		g_KeyMapping.assign('R', eKey::r);
		g_KeyMapping.assign('S', eKey::s);
		g_KeyMapping.assign('T', eKey::t);

		g_KeyMapping.assign('U', eKey::u);
		g_KeyMapping.assign('V', eKey::v);
		g_KeyMapping.assign('W', eKey::w);
		g_KeyMapping.assign('X', eKey::x);

		g_KeyMapping.assign('Y', eKey::y);
		g_KeyMapping.assign('Z', eKey::z);

		// numbers *.assign(_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		g_KeyMapping.assign('1', eKey::_1);
		g_KeyMapping.assign('2', eKey::_2);
		g_KeyMapping.assign('3', eKey::_3);
		g_KeyMapping.assign('4', eKey::_4);

		g_KeyMapping.assign('5', eKey::_5);
		g_KeyMapping.assign('6', eKey::_6);
		g_KeyMapping.assign('7', eKey::_7);
		g_KeyMapping.assign('8', eKey::_8);

		g_KeyMapping.assign('9', eKey::_9);
		g_KeyMapping.assign('0', eKey::_0);

		// function keys
		g_KeyMapping.assign(VK_F1, eKey::f1);
		g_KeyMapping.assign(VK_F2, eKey::f2);
		g_KeyMapping.assign(VK_F3, eKey::f3);
		g_KeyMapping.assign(VK_F4, eKey::f4);

		g_KeyMapping.assign(VK_F5, eKey::f5);
		g_KeyMapping.assign(VK_F6, eKey::f6);
		g_KeyMapping.assign(VK_F7, eKey::f7);
		g_KeyMapping.assign(VK_F8, eKey::f8);

		g_KeyMapping.assign(VK_F9, eKey::f9);
		g_KeyMapping.assign(VK_F10, eKey::f10);
		g_KeyMapping.assign(VK_F11, eKey::f11);
		g_KeyMapping.assign(VK_F12, eKey::f12);

		// symbol keys (assuming US standard keyboard)
		g_KeyMapping.assign(VK_OEM_1, eKey::semicolon);
		g_KeyMapping.assign(VK_OEM_2, eKey::questionmark);
		g_KeyMapping.assign(VK_OEM_3, eKey::tilde);
		g_KeyMapping.assign(VK_OEM_4, eKey::brace_open);
		g_KeyMapping.assign(VK_OEM_5, eKey::vertical_pipe);
		g_KeyMapping.assign(VK_OEM_6, eKey::brace_close);
		g_KeyMapping.assign(VK_OEM_7, eKey::double_quote);
		g_KeyMapping.assign(VK_OEM_8, eKey::oem_8);

		g_KeyMapping.assign(VK_OEM_PLUS,   eKey::plus);
		g_KeyMapping.assign(VK_OEM_MINUS,  eKey::minus);
		g_KeyMapping.assign(VK_OEM_COMMA,  eKey::comma);
		g_KeyMapping.assign(VK_OEM_PERIOD, eKey::period);
					
		// navigational keys
		g_KeyMapping.assign(VK_UP,    eKey::up);
		g_KeyMapping.assign(VK_DOWN,  eKey::down);
		g_KeyMapping.assign(VK_LEFT,  eKey::left);
		g_KeyMapping.assign(VK_RIGHT, eKey::right); 

		g_KeyMapping.assign(VK_PRIOR, eKey::pg_up);
		g_KeyMapping.assign(VK_NEXT,  eKey::pg_down);
		g_KeyMapping.assign(VK_HOME,  eKey::home);
		g_KeyMapping.assign(VK_END,   eKey::end);

		g_KeyMapping.assign(VK_INSERT, eKey::ins);
		g_KeyMapping.assign(VK_DELETE, eKey::del);

		// everything else
		g_KeyMapping.assign(VK_CONTROL, eKey::ctrl);
		g_KeyMapping.assign(VK_MENU,    eKey::alt);
		g_KeyMapping.assign(VK_SHIFT,   eKey::shift);
		g_KeyMapping.assign(VK_SPACE,   eKey::space);

		g_KeyMapping.assign(VK_TAB,    eKey::tab);
		g_KeyMapping.assign(VK_RETURN, eKey::enter);
		g_KeyMapping.assign(VK_ESCAPE, eKey::escape);
		g_KeyMapping.assign(VK_BACK,   eKey::backspace);
	}

	auto find_key_code(WPARAM wp) {
		if (auto* result = g_KeyMapping[wp])
			return *result;
		else
			return scimitar::input::Keyboard::eKey::undefined;
	}

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

		if (g_KeyMapping.is_empty())
			populate_keymapping();

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

		// extend drawable area over titlebar+border
		MARGINS margins{ 0, 0, 0, 1 };
		DwmExtendFrameIntoClientArea(m_NativeHandle, &margins);

		// determine DPI
		{
			auto value = GetDpiForWindow(m_NativeHandle);
			if (value == 0)
				throw std::runtime_error("Failed to get window DPI");

			m_Dpi = static_cast<float>(value);
		}

		// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
		SetWindowPos(
			m_NativeHandle,
			nullptr,				// insertAfter
			0,                      // X (left)
			0,                      // Y (top)
			0,                      // cx (width in pixels)
			0,                      // cy (height in pixels)
			SWP_NOMOVE        |     // retain the current position (ignore x/y params)
			SWP_NOSIZE        |		// retain the current size     (ignore cx/cy params)
			SWP_NOZORDER      |		// retain the current Z-order  (ignore insertAfter param)
			SWP_NOOWNERZORDER |		// does not change the owner window's position in the Z order
			SWP_FRAMECHANGED        // send WM_NCCALCSIZE even if the actual size isn't changed
		);

		// setup virtual keyboard/mouse for this window
		{
			auto* input_system = core::Engine::instance().get<Input>();

			m_Keyboard = std::make_unique<Keyboard>(input_system);
			m_Mouse    = std::make_unique<Mouse>(input_system);
		}

		// create the vulkan surface
		/* {
			vk::Win32SurfaceCreateInfoKHR info;

			info
				.setHinstance(GetModuleHandle(NULL))
				.setHwnd(m_NativeHandle);

			m_Surface = m_Owner->get_vk_instance().createWin32SurfaceKHRUnique(info);

			if (!m_Surface)
				throw std::runtime_error("Failed to create vulkan surface");
		}*/
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
		case WM_DESTROY: {
				// remove the associated object pointer to prevent further cascades
				SetWindowLongPtr(window, GWLP_USERDATA, NULL);

				m_Owner->close(this);
			} break;

		case WM_CREATE: {
				auto* info = (CREATESTRUCT*)lp;
				RECT rect;

				rect.left = info->x;
				rect.top = info->y;
				rect.right = info->x + info->cx;
				rect.bottom = info->y + info->cy;

				// should update layout here
			} break;

			// https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-nccalcsize
		case WM_NCCALCSIZE: {
				if (wp == TRUE)
					return 0; // old client area is preserved and is aligned with upper-left corner of the new client area
			} break;

		case WM_DPICHANGED: {
				m_Dpi = static_cast<float>(LOWORD(wp));

				gLog << "Dpi changed to " << m_Dpi;

				m_ShouldLayout = true;
			} break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
				auto key = find_key_code(wp);
				m_Keyboard->set_state(key, true);
			} break;

		case WM_KEYUP:
		case WM_SYSKEYUP: {
				auto key = find_key_code(wp);
				m_Keyboard->set_state(key, false);
			} break;
		}


		return -1;
	}
}