#pragma once

#include <string>
#include "../dependencies.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"

namespace scimitar {
	class OS;
}

namespace scimitar::os {
	/*
	*	NOTE at the moment, this is pretty much made with win32 in mind; once I actually have decent other 
	*        systems to develop for this will probably need some reworking
	*/

	class Window {
	public:
#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
		using Handle = HWND;

		LRESULT CALLBACK win_proc(
			HWND   window,
			UINT   msg,
			WPARAM wp,
			LPARAM lp
		);
#else
	#error "not sure what this is on other platforms"
#endif
		using Keyboard = input::Keyboard;
		using Mouse    = input::Mouse;

		Window(
			const std::string& title, 
			uint32_t           width, 
			uint32_t           height,
			bool               windowed, 
			int                display_device_idx, // aka MonitorIndex
			OS*                owner               // because we need the vkInstance, and to notify 'window close' events
		);

		// no-copy, no-move
		// NOTE we're storing a pointer to this object with the native window, 
		//      which makes moving it a bunc more complicated. Avoiding it entirely.
		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&&)      = delete;
		Window& operator = (Window&&)      = delete;

		Handle get_native_handle() const;

		bool is_main_window() const;

		const Keyboard* get_keyboard() const;
		const Mouse*    get_mouse() const;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		//vk::SurfaceKHR getSurface() const;

	private:
		OS*    m_Owner        = nullptr;
		Handle m_NativeHandle = nullptr;

		inline static Handle s_MainWindow = nullptr;

		uint32_t m_Width  = 0;
		uint32_t m_Height = 0;
		float    m_Dpi    = 0.0f; // horizontal dpi

		bool m_ShouldLayout = false; // when true, the layout will be updated on next update

		std::unique_ptr<Keyboard> m_Keyboard;
		std::unique_ptr<Mouse>    m_Mouse;

		//vk::UniqueSurfaceKHR m_Surface;
	};
}