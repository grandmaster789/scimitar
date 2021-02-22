#include "debugger.h"
#include "string_util.h"

#if SCIMITAR_PLATFORM ==SCIMITAR_PLATFORM_WINDOWS

namespace scimitar::util {
	bool debugger_present() noexcept {
		return IsDebuggerPresent();
	}

	void debugger_log(std::string_view text) noexcept {
		auto message   = std::string(text) + "\r\n";
		//auto messageWS = to_wstring(message);

		//OutputDebugStringW(messageWS.data());
	}

	void debugger_dialog(
		std::string_view caption,
		std::string_view text
	) noexcept {
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