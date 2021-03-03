#include "debugger.h"
#include "string_util.h"
#include "codec/utf.h"
#include "../preprocessor.h"

#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS

namespace scimitar::util {
	bool debugger_present() noexcept {
		return IsDebuggerPresent();
	}

	void debugger_log(std::string_view text) noexcept {
		auto message   = std::string(text) + "\r\n";
		auto messageWS = to_wstring(message);

		OutputDebugStringW(messageWS.data());
	}

	void debugger_dialog(
		std::string_view caption,
		std::string_view text
	) noexcept {
		std::wstring cap_ws = to_wstring(caption);
		std::wstring msg_ws = to_wstring(text);

		// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxw
		MessageBoxW(
			nullptr,       // HWND parent
			msg_ws.data(), // text
			cap_ws.data(), // caption
			MB_APPLMODAL | // type flags 
			MB_OK        |
			MB_ICONERROR 
		);
	}

	void debugger_fatal(
		const char* source_file,
		int source_line
	) {
		debugger_fatal(
			source_file, 
			source_line,
			"Unknown cause"
		);
	}
}

#elif SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_LINUX
#else
	#error "not implemented"
#endif