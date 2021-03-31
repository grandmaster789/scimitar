#pragma once

#include "concepts.h"

// NOTE casting occurs often enough to put it in the project namespace
namespace scimitar { 
	template <
		std::signed_integral T,
		std::signed_integral U
	>
	[[nodiscard]] constexpr T narrow_cast(U value) noexcept; // narrow_cast<int>(long)

	template <
		std::signed_integral   T,
		std::unsigned_integral U
	>
	[[nodiscard]] constexpr T narrow_cast(U value) noexcept; // narrow_cast<int>(uint32_t)

	template <
		std::signed_integral T, 
		std::floating_point  U
	>
	[[nodiscard]] constexpr T narrow_cast(U value) noexcept; // narrow_cast<int>(float)

	
}

#include "cast.inl"