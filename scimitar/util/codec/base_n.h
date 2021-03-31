#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <span>

namespace scimitar::util::codec {
	namespace detail {
		//
		// https://tools.ietf.org/html/rfc4648
		// https://tools.ietf.org/html/rfc1924
		// https://en.wikipedia.org/wiki/Base64
		// https://en.wikipedia.org/wiki/Ascii85
		// https://github.com/git/git/blob/master/base85.c
		// (common implementations are Base64, Base32, Base36, Base58, Base62 and Base85)
		// (Base64 is popular on the web, Base85 rfc1924 is used in git/json)
		// Theoretically this should support encoding from/to up to base127, but concrete
		// alphabets are provided for common usage
		struct BaseN {
			bool     m_CaseSensitive = false;
			uint64_t m_Radix;
			char     m_PaddingCharacter;

			static_assert(sizeof(int8_t) == sizeof(char));

			std::array<int8_t, 256> m_CharacterToInt = {}; // -2 on invalid characters, -1 on whitespace
			std::array<char, 127>   m_IntToCharacter = {}; 

			template <size_t N>
			requires (N < 128)
			constexpr BaseN(
				const char (&text)[N],				       // null terminated string with le alphabet
				bool       is_case_sensitive = (N >= 33),  // base16, base32, base32hex should treat upper and lower case letters the same
				char       padding_character = '\0'		   // explicit padding characters are really only required for base64
			) noexcept;

			constexpr char   from_int (int8_t x) const noexcept;
			 int8_t from_char(char c)   const noexcept;
		};
	}

	constexpr auto c_Base2             = detail::BaseN { "01" };
	constexpr auto c_Base8             = detail::BaseN { "01234567" };
	constexpr auto c_Base10            = detail::BaseN { "0123456789" };
	constexpr auto c_Base16            = detail::BaseN { "0123456789ABCDEF" };
	constexpr auto c_Base32_rfc4648    = detail::BaseN { "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"}; // https://tools.ietf.org/html/rfc4648#page-8
	constexpr auto c_Base32hex_rfc4648 = detail::BaseN { "0123456789ABCDEFGHIJKLMNOPQRSTUV"}; // https://tools.ietf.org/html/rfc4648#page-10
	constexpr auto c_Base64_rfc4648    = detail::BaseN { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", true, '=' }; // https://tools.ietf.org/html/rfc4648#page-5
	constexpr auto c_Base64url_rfc4648 = detail::BaseN { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_", true, '=' }; // https://tools.ietf.org/html/rfc4648#page-7
	constexpr auto c_Base85_rfc1924    = detail::BaseN { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~",   true }; // https://tools.ietf.org/html/rfc1924
}

#include "base_n.inl"