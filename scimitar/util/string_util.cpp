#include "string_util.h"

namespace scimitar::util {
	[[nodiscard]] constexpr bool is_upper(char c) noexcept {
		return
			(c >= 'A') &&
			(c <= 'Z');
	}

	[[nodiscard]] constexpr bool is_lower(char c) noexcept {
		return
			(c >= 'a') &&
			(c <= 'z');
	}

	[[nodiscard]] constexpr bool is_alpha(char c) noexcept {
		return
			is_upper(c) ||
			is_lower(c);
	}

	[[nodiscard]] constexpr bool is_digit(char c) noexcept {
		return
			(c >= '0') &&
			(c <= '9');
	}

	[[nodiscard]] constexpr bool is_alphanum(char c) noexcept {
		return
			is_alpha(c) ||
			is_digit(c);
	}

	// https://en.cppreference.com/w/cpp/language/escape
	[[nodiscard]] constexpr bool is_linefeed(char c) noexcept {
		return
			(c == '\n') ||	// new line        (should be the most common by far)
			(c == '\r') ||  // carriage return (fairly common in Windows or old Apple plaintext)
			(c == '\f') ||	// form feed       (exceedingly rare, I've never seen this in the wild)
			(c == '\v');	// vertical tab    (exceedingly rare, I've never seen this in the wild)
	}

	[[nodiscard]] constexpr bool is_whitespace(char c) noexcept {
		return
			(c == ' ')  || // space
			(c == '\t') || // tab
			is_linefeed(c);
	}

	[[nodiscard]] constexpr bool is_number_begin(char c) noexcept {
		return
			is_digit(c) ||
			(c == '+')  ||
			(c == '-');
	}

	[[nodiscard]] constexpr bool is_name_begin(char c) noexcept {
		return
			is_alpha(c) ||
			(c == '_')  ||
			(c == '$');
	}

	[[nodiscard]] constexpr bool is_name_next(char c) noexcept {
		return
			is_alphanum(c) ||
			(c == '_')     ||
			(c == '$');
	}

	[[nodiscard]] constexpr bool is_quote(char c) noexcept {
		return
			(c == '\'') ||
			(c == '"')  ||
			(c == '`');
	}

	[[nodiscard]] constexpr bool is_open_bracket(char c) noexcept {
		return
			(c == '(') ||
			(c == '[') ||
			(c == '{');
	}

	[[nodiscard]] constexpr bool is_close_bracket(char c) noexcept {
		return
			(c == ')') ||
			(c == ']') ||
			(c == '}');
	}

	[[nodiscard]] constexpr bool is_operator(char c) noexcept {
		return
			(c == '~')  ||
			(c == '!')  ||
			(c == '@')  ||
			(c == '#')  ||
			(c == '%')  ||
			(c == '^')  ||
			(c == '&')  ||
			(c == '*')  ||
			(c == '-')  ||
			(c == '+')  ||
			(c == '=')  ||
			(c == '|')  ||
			(c == '\\') ||
			(c == ';')  ||
			(c == ':')  ||
			(c == ',')  ||
			(c == '.')  ||
			(c == '/')  ||
			(c == '?');
	}

	[[nodiscard]] std::string to_lower(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size());

		for (const auto c : sv) {
			if (is_upper(c))
				result += (c - 'A' + 'a');
			else
				result += c;
		}

		return result;

	}

	[[nodiscard]] std::string to_upper(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size());

		for (const auto c : sv) {
			if (is_lower(c))
				result += (c - 'a' + 'A');
			else
				result += c;
		}

		return result;
	}

	[[nodiscard]] std::string normalize_linefeeds(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size());

		for (auto c : sv) {
			if (
				(c == '\r') ||
				(c == '\f') ||
				(c == '\v')
			)
				[[unlikely]]
				continue;  
			else
				[[likely]]
				result += c;
		}

		return result;
	}
}