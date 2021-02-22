#pragma once

#include "helpers.h"

#include <algorithm>
#include <cmath>
#include <concepts>

namespace scimitar::math {
	template <typename T>
	T square(T value) noexcept {
		return value * value;
	}

	template <typename T>
	T cubed(T value) noexcept {
		return value * value * value;
	}

	template <std::floating_point T>
	constexpr T smoothstep(T value) noexcept {
		value = std::clamp(value, T{ 0.0 }, T{ 1.0 });

		return
			square(value) * (T{ 3.0 } - T{ 2.0 } * value);
	}

	template <std::floating_point T>
	constexpr T inverse_smoothstep(T value) noexcept {
		return
			T{ 0.5 } -
			std::sin(
				std::asin(
					T{ 1.0 } - T{ 2.0 } * value
				) / T{ 3.0 }
			);
	}

	template <typename T, std::floating_point U>
	T mix(
		U value,
		const T& min_result,
		const T& max_result
	) {
		if (value >= T{ 1.0 })
			return max_result;

		if (value <= T{ 0.0 })
			return min_result;

		return min_result + (max_result - min_result) * value;
	}
}