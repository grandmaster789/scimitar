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
	template <
		typename            T,
		std::floating_point U
	>
	T mix(
		U        value,		 // [0..1]
		const T& min_result, // returned when value <= 0
		const T& max_result  // returned when value >= 1
	);

}

#include "helpers.inl"