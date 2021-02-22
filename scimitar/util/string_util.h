#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <utility>

namespace scimitar::util {
	[[nodiscard]] constexpr bool is_upper        (char c) noexcept; // A-Z
	[[nodiscard]] constexpr bool is_lower        (char c) noexcept; // a-Z
	[[nodiscard]] constexpr bool is_alpha        (char c) noexcept; // A-Za-z
	[[nodiscard]] constexpr bool is_digit        (char c) noexcept; // 0-9
	[[nodiscard]] constexpr bool is_alphanum     (char c) noexcept; // A-Za-z0-9
	[[nodiscard]] constexpr bool is_linefeed     (char c) noexcept; // \n\r\f\v
	[[nodiscard]] constexpr bool is_whitespace   (char c) noexcept; // ' '\t\n\r\f\v
	[[nodiscard]] constexpr bool is_number_begin (char c) noexcept; // 0-9+-
	[[nodiscard]] constexpr bool is_name_begin   (char c) noexcept; // A-Za-z_$
	[[nodiscard]] constexpr bool is_name_next    (char c) noexcept; // A-Za-z0-9_$
	[[nodiscard]] constexpr bool is_quote        (char c) noexcept; // '"`
	[[nodiscard]] constexpr bool is_open_bracket (char c) noexcept; // ([{
	[[nodiscard]] constexpr bool is_close_bracket(char c) noexcept; // )]}
	[[nodiscard]] constexpr bool is_operator     (char c) noexcept; // ~!@#%^&*<>?/,.:;|\+-=

	[[nodiscard]] std::string to_lower           (std::string_view sv) noexcept;
	[[nodiscard]] std::string to_upper           (std::string_view sv) noexcept;
	[[nodiscard]] std::string normalize_linefeeds(std::string_view sv) noexcept; // get rid of all line feeds except for \n

	// search the haystack for all needles, return the first matched range
	template <typename T, typename... Ts>
	[[nodiscard]] std::pair<size_t, size_t> find_first_string(
		std::string_view haystack,
		size_t           offset,
		const T&         needle_head,
		const Ts&...     needle_tail
	) noexcept;

	template <typename... Ts>
	[[nodiscard]] std::vector<std::string> split(
		std::string_view haystack, 
		const Ts&...     needles
	) noexcept;
}

#include "string_util.inl"