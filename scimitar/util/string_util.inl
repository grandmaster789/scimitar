#pragma once

#include "string_util.h"

namespace scimitar::util {
	template <typename T, typename... Ts>
	[[nodiscard]] std::pair<size_t, size_t> find_first_string(
		std::string_view haystack,
		size_t           offset,
		const T&         needle_head,
		const Ts&...     needle_tail
	) noexcept {
		size_t first = haystack.find(needle_head, offset); // https://en.cppreference.com/w/cpp/string/basic_string_view/find
		size_t last  = first + std::size(needle_head);

		if (first == std::string_view::npos) {
			first = std::size(haystack);
			last  = std::size(haystack);
		}

		if constexpr (sizeof...(Ts) != 0) {
			const auto [x, y] = find_first_string(
				haystack, 
				offset, 
				needle_tail...
			); // tail recursion whoo

			if (x < first) {
				first = x;
				last  = y;
			}
		}

		return { first, last };
	}

	template <typename... Ts>
	[[nodiscard]] std::vector<std::string> split(
		std::string_view haystack,
		const Ts&... needles
	) noexcept {
		std::vector<std::string> result;

		std::string_view::size_type cursor = 0;

		while (cursor != std::size(haystack)) {
			const auto [x, y] = find_first_string(haystack, cursor, needles...);

			result.emplace_back(
				haystack.substr(cursor, x - cursor)
			);

			cursor = y;
		}

		return result;
	}

	namespace traits {
		inline constexpr void ByteChar::assign(
			char_type& result,
			const char_type& value
		) noexcept {
			result = value;
		}

		inline constexpr bool ByteChar::eq(
			char_type a,
			char_type b
		) noexcept {
			return (static_cast<uint8_t>(a) == static_cast<uint8_t>(b));
		}
	}
}