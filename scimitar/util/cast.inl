#pragma once

#include "cast.h"
#include <cassert>
#include <numeric>

namespace scimitar {
	template <std::signed_integral T, std::signed_integral U>
	[[nodiscard]] constexpr T narrow_cast(U value) noexcept {
		constexpr auto minimum = std::numeric_limits<T>::lowest();
		constexpr auto maximum = std::numeric_limits<T>::max();

		assert(value >= minimum);
		assert(value <= maximum);

		return static_cast<T>(value);
	}

	template <std::signed_integral T, std::floating_point U>
	[[nodiscard]] constexpr T narrow_cast(U value) noexcept {
		constexpr auto minimum = std::numeric_limits<T>::lowest();
		constexpr auto maximum = std::numeric_limits<T>::max();

		assert(value >= minimum);
		assert(value <= maximum);

		return static_cast<T>(value);
	}
}