#pragma once

#include "biguint.h"

namespace scimitar::util {
	template <util::cUnsigned T, int N>
	template <int M>
	requires (M < N)
	Biguint<T, N>::Biguint(const Biguint<T, M>& x) noexcept {
		int i = 0;

		for (; i < M; ++i)
			m_Digits[i] = x.m_Digits[i];

		for (; i < N; ++i)
			m_Digits[i] = 0;
	}

	template <util::cUnsigned T, int N>
	template <util::cIntegral U>
	Biguint<T, N>::Biguint(U value) 
		noexcept 
	{
		if constexpr (sizeof(U) > sizeof(T)) {
			// ie Biguint<uint32_t, 2>(uint64_t some_value)			
			for (int i = 0; i < m_NumDigits; ++i)
				m_Digits[i] = static_cast<T>(value >> i * m_BitsPerDigit);

			// do we want to warn about truncation?
		}

		if constexpr (sizeof(U) <= sizeof(T)) {
			// ie Biguint<uint64_t, 2>(int32_t some_value)

			m_Digits[0] = static_cast<T>(value);

			for (int i = 1; i < m_NumDigits; ++i)
				m_Digits[i] = 0;
		}
	}

	template <util::cUnsigned T, int N>
	Biguint<T, N>::Biguint(std::string_view sv, int base) noexcept {
	}

	template <util::cUnsigned T, int N>
	template <int M>
	requires (M < N)
	Biguint<T, N>& Biguint<T, N>::operator = (const Biguint<T, M>& x) noexcept {
		int i = 0;

		for (; i < M; ++i)
			m_Digits[i] = x.m_Digits[i];

		for (; i < N; ++i)
			m_Digits[i] = 0;

		return *this;
	}

	template <util::cUnsigned T, int N>
	template <util::cIntegral U>
	Biguint<T, N>& Biguint<T, N>::operator = (U value)
		noexcept
	{
		if constexpr (sizeof(U) > sizeof(T)) {
			// ie Biguint<uint32_t, 2>(uint64_t some_value)			
			for (int i = 0; i < m_NumDigits; ++i)
				m_Digits[i] = static_cast<T>(value >> i * m_BitsPerDigit);

			// do we want to warn about truncation?
		}

		if constexpr (sizeof(U) <= sizeof(T)) {
			// ie Biguint<uint64_t, 2>(int32_t some_value)

			m_Digits[0] = static_cast<T>(value);

			for (int i = 1; i < m_NumDigits; ++i)
				m_Digits[i] = 0;
		}

		return *this;
	}

	template <util::cUnsigned T, int N>
	Biguint<T, N>::operator bool() const noexcept {
		for (auto digit : m_Digits)
			if (digit)
				return true;

		return false;
	}

	template <util::cUnsigned T, int N>
	[[nodiscard]]
	int biguint_cmp(
		const Biguint<T, N>& a, 
		const Biguint<T, N>& b
	) noexcept {
		for (int i = N - 1; i >= 0; --i) {
			auto digit_a = a.m_Digits[i];
			auto digit_b = b.m_Digits[i];

			if (digit_a < digit_b)
				return -1; // A is smaller than B

			if (digit_a > digit_b)
				return 1; // A is greater than B
		}

		return 0; // A and B are equal
	}

	template <util::cUnsigned T, int N>
	[[nodiscard]]
	bool operator == (
		const Biguint<T, N>& a, 
		const Biguint<T, N>& b
	) noexcept {
		return (biguint_cmp(a, b) == 0);
	}
}