#pragma once

#include "helpers.h"

#include <algorithm>
#include <cmath>
#include <concepts>
#include <bit>
#include <numeric>
#include <iterator>

namespace scimitar::math {
	template <typename T>
	constexpr T square(T value) noexcept {
		return value * value;
	}

	template <typename T>
	constexpr T cubed(T value) noexcept {
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

	constexpr bool approx_equal(float a, float b) noexcept {
		constexpr int64_t epsilon = 5;

		// from https://en.wikipedia.org/wiki/IEEE_754
		// ('float' should pretty much be the binary32 case)
		//   [1  bit sign]
		//   [8  bits exponent]
		//   [23 bits fraction]

		static_assert(sizeof(float) == sizeof(int32_t));

		// treat as integer values
		int32_t aa = std::bit_cast<int32_t>(a);
		int32_t bb = std::bit_cast<int32_t>(b);

		// get rid of the sign bit and extend bit range to prevent possible overflows
		// ~> 23+8 = 31bit = 0x7FFF'FFFF
		int64_t aaa = static_cast<int64_t>(aa & 0x7FFF'FFFF);
		int64_t bbb = static_cast<int64_t>(bb & 0x7FFF'FFFF);

		// if both floats have a the same sign we need to subtract; add otherwise
		int64_t delta;

		if ((a < 0) == (b < 0))
			delta = aaa - bbb;
		else
			delta = aaa + bbb;

		// now we can just test within the epsilon range
		return
			(delta <  epsilon) &&
			(delta > -epsilon);
	}

	template <typename T>
	constexpr T median(T a, T b, T c) noexcept {
		return std::clamp(
			c, 
			std::min(a, b), 
			std::max(a, b)
		);
	}

	template <typename It>
	auto mean(It first, It last) {
		constexpr auto zero = static_cast<typename std::iterator_traits<It>::value_type>(0);

		auto sum         = std::reduce(first, last, zero);
		auto num_entries = static_cast<decltype(sum)>(
			std::distance(first, last)
		);

		if (num_entries > 0)
			return sum / num_entries;
		else
			return zero;
	}

	template <typename It>
	auto variance(It first, It last) {
		constexpr auto zero = static_cast<typename std::iterator_traits<It>::value_type>(0);

		auto u   = mean(first, last);
		auto sum = std::accumulate(
			first,
			last,
			zero,
			[u](const auto& accumulator, const auto& entry) {
				return accumulator + square(entry - u);
			}
		);

		const auto num_entries = static_cast<decltype(sum)>(
			std::distance(first, last)
		);

		if (num_entries > 0)
			return sum / num_entries;
		else
			return zero;
	}
}