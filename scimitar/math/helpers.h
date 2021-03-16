#pragma once

namespace scimitar::math {
	template <typename T>
	T square(T value) noexcept;

	template <typename T>
	T cubed(T value) noexcept;

	// https://en.wikipedia.org/wiki/Smoothstep
	template <std::floating_point T>
	constexpr T smoothstep(T value) noexcept; 

	template <std::floating_point T>
	constexpr T inverse_smoothstep(T value) noexcept;

	// linear interpolation
	template <typename T, std::floating_point U>
	T mix(
		U        value,		 // [0..1]
		const T& min_result, // returned when value <= 0
		const T& max_result  // returned when value >= 1
	);

	constexpr bool approx_equal(float a, float b) noexcept; // at most 10 smallest step float value from each other

	// https://en.wikipedia.org/wiki/Median (sort of)
	template <typename T>
	constexpr T median(
		T a, 
		T b, 
		T c
	) noexcept; // given 3 values, yield the one in the middle

	// https://en.wikipedia.org/wiki/Arithmetic_mean
	template <typename It>
	auto mean(It first, It last);

	// https://en.wikipedia.org/wiki/Variance
	// https://en.wikipedia.org/wiki/Standard_deviation
	// (discrete random variable)
	// NOTE as part of calculating this a mean is determined as well... may not be quite optimal
	template <typename It>
	auto variance(It first, It last); 
}

#include "helpers.inl"