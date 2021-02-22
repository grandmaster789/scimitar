#pragma once

#include "../preprocessor.h"
#include <string_view>

#if SCIMITAR_OS == SCIMITAR_OS_WINDOWS
	#define debug_break DebugBreak
#elif SCIMITAR_OS == SCIMITAR_OS_LINUX
	#define debug_break __builtin_trap // should work for both GCC and Clang
#else
	#error "not implemented"
#endif

namespace scimitar::util {
	bool debugger_present() noexcept;
	void debugger_log   (std::string_view text) noexcept;
	void debugger_dialog(std::string_view caption, std::string_view text) noexcept;
	void debugger_fatal (
		const char*      source_file, 
		int              source_line
	);
	
	template <typename... Ts>
	void debugger_log(std::string_view fmt, Ts&&... args) noexcept;
	
	template <typename... Ts>
	void debugger_dialog(
		std::string_view caption,
		std::string_view fmt,
		Ts&&...          args
	) noexcept;

	template <typename... Ts>
	void debugger_fatal(
		const char*      source_file,
		int              source_line,
		std::string_view fmt,
		Ts&&...          args
	);
}

#define debugger_abort(...) ::scimitar::util::debugger_fatal(__FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#include "debugger.inl"