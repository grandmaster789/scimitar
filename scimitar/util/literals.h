#pragma once

#include <string>
#include <chrono>
#include <cstddef>

namespace scimitar {
	/*
	*	std::literals::chrono_literals
	*		d   ~> day in month
	*		h   ~> hour
	*		min ~> minute
	*		ms  ~> millisecond  (10^-3 s)
	*		ns  ~> nanosecond	(10^-9 s)
	*		s   ~> second
	*		us  ~> microsecond  (10^-6 s)
	*		y   ~> year
	* 
	*	std::literals::complex_literals
	*		i   ~> imaginary component <double>
	*		if  ~> imaginary component <float>
	*		il  ~> imaginary component <long double>
	* 
	*	std::literals::string_literals
	*		s	~> (const char*)     ~> std::string
	*			   (const char8_t*)  ~> std::u8string
	*			   (const char16_t*) ~> std::u16string
	*			   (const char32_t*) ~> std::u32string
	*		       (const wchar_t*)  ~> std::wstring
	* 
	*	std::literals::string_view_literals
	*		sv  ~> (const char*)     ~> std::string_view
	*			   (const char8_t*)  ~> std::u8string_view
	*			   (const char16_t*) ~> std::u16string_view
	*			   (const char32_t*) ~> std::u32string_view
	*              (const wchar_t*)  ~> std::wstring_view
	*/
	using namespace std::literals;

	using ssize_t = std::ptrdiff_t; // signed size type

#define ssizeof(x) (static_cast<ssize_t>(sizeof(x)))

	constexpr ssize_t operator "" _z (unsigned long long value) noexcept; // 123_z  ~> (ssize_t)(123)
	constexpr size_t  operator "" _uz(unsigned long long value) noexcept; // 123_uz ~> (size_t) (123)
}

#include "literals.inl"