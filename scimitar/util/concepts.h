#pragma once

#include <type_traits>
#include <concepts>
#include <string>
#include <string_view>

namespace scimitar::util {
	template <typename T> concept cArithmetic    = std::is_arithmetic_v<T>;
	template <typename T> concept cIntegral      = std::is_integral_v<T>;
	template <typename T> concept cUnsigned      = std::is_unsigned_v<T>;
	template <typename T> concept cSigned        = std::is_signed_v<T>;
	template <typename T> concept cFloatingPoint = std::is_floating_point_v<T>;
	template <typename T> concept cPointer       = std::is_pointer_v<T>;
	template <typename T> concept cReference     = std::is_reference_v<T>;
	template <typename T> concept cLValueRef     = std::is_lvalue_reference_v<T>;
	template <typename T> concept cRValueRef     = std::is_rvalue_reference_v<T>;

	template <typename T>
	concept cToString = requires(T value) {
		{ to_string(value) } -> std::convertible_to<std::string>;
	};

	/*
	template <typename T>
	concept cFromString = requires(T value) {
		{ from_string(std::string_view{}) } -> std::convertible_to<T>;
	};
	*/

	template <typename T, typename U>
	concept cStaticCast = requires(T value) {
		{ static_cast<U>(value) }->std::convertible_to<U>;
	};

	template <typename T>
	concept cSize = requires(T value) {
		{ size(value) } -> std::convertible_to<size_t>;
	};
}