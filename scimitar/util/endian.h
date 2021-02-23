#pragma once

#include "concepts.h"

namespace scimitar::util {
	template <cUnsigned T>      [[nodiscard]] T byte_swap(T value) noexcept;
	template <cSigned T>        [[nodiscard]] T byte_swap(T value) noexcept;
	template <cFloatingPoint T> [[nodiscard]] T byte_swap(T value) noexcept;
}

#include "endian.inl"