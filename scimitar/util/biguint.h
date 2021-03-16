#pragma once

#include "concepts.h"

#include <cstdint>
#include <array>
#include <string_view>

namespace scimitar::util {
	// 
	// 
	// Somewhat based around UUIDs (https://en.wikipedia.org/wiki/Universally_unique_identifier) 
	// (as in 128-bit unsinged integer composed out of 2x uint64_t == Biguint<uint64_t, 2>)
	// (this is also the reason it's in the util namespace)
	template <util::cUnsigned T, int N>
	class Biguint {
	public:
		using Digit = T;

		Biguint() = default;

		template <int M>
		requires (M < N)
		Biguint(const Biguint<T, M>& x) noexcept;                      // implicit construction from smaller biguint

		template <util::cIntegral U>
		Biguint(U value) noexcept;                                     // implicit construction from native integer types
		
		explicit Biguint(std::string_view sv, int base = 10) noexcept; // explicit construction from string (assume base decimal default)

		template <int M>
		requires (M < N)
		Biguint& operator = (const Biguint<T, M>& x) noexcept;         // assignment from a smaller biguint

		template <util::cIntegral U>
		Biguint& operator = (U value) noexcept;                        // assign from native integer types

		explicit operator bool() const noexcept;

		[[nodiscard]]
		friend int biguint_cmp(const Biguint& a, const Biguint& b) noexcept;

		[[nodiscard]]
		friend bool operator == (const Biguint& a, const Biguint& b) noexcept;

	private:
		static_assert(sizeof(int16_t) == 2); // ensure that we have 8 bits per byte

		static constexpr int m_NumDigits    = N;
		static constexpr int m_BitsPerDigit = sizeof(Digit) * 8;
		static constexpr int m_NumBits      = m_NumDigits * m_BitsPerDigit;

		std::array<Digit, m_NumDigits> m_Digits;
	};

	using ubig128 = Biguint<uint64_t, 2>;
	using uuid    = Biguint<uint64_t, 2>;
}

#include "biguint.inl"