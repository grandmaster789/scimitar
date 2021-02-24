#pragma once

#include "debugger.h"
#include <fmt/format.h>
#include <cstdlib>

namespace scimitar::util {
	template <typename...Ts>
	void debugger_log(
		std::string_view fmt,
		Ts&&...          args
	) noexcept {
		debugger_log(
			fmt::format(fmt, std::forward<Ts>(args)...).data()
		);
	}

	template <typename...Ts>
	void debugger_dialog(
		std::string_view caption,
		std::string_view fmt,
		Ts&&...          args
	) noexcept {
		debugger_dialog(
			caption,
			fmt::format(fmt, std::forward<Ts>(args)...).data()
		);
	}

	template <typename... Ts>
	void debugger_fatal(
		const char*      source_file,
		int              source_line,
		std::string_view fmt,
		Ts&&...          args
	) {
		std::string message = fmt::format(fmt, std::forward<Ts>(args)...);

		if (debugger_present()) {
			debugger_log("{}:{} {}", source_file, source_line, message);
			debug_break();
		}
		else
			debugger_dialog("Fatal", "{}:{} {}", source_file, source_line, message);

		std::abort(); // https://en.cppreference.com/w/c/program/abort
	}
}