#pragma once

#include <cstddef>

namespace scimitar::util {
	// https://en.wikipedia.org/wiki/Windows-1252
	//
	[[nodiscard]] constexpr char32_t codepage1252_to_utf32(char c) noexcept;
}