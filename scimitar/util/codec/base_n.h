#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <span>

namespace scimitar::util::codec {
	namespace detail {
		//
		// https://en.wikipedia.org/wiki/Base64
		// (common implementations are Base64, Base32, Base36, Base58, Base62 and Base85)
		// (Base64 is popular on the web)
		//
		struct BaseN_Alphabet {
			bool     m_CaseSensitive = false;
			uint64_t m_Radix;
			char     m_PaddingCharacter;

			static_assert(sizeof(int8_t) == sizeof(char));

			std::array<int8_t, 256> m_CharacterToInt = {}; // -2 on invalid characters, -1 on whitespace
			std::array<char, 127>   m_IntToCharacter = {}; 

			template <size_t N>
			requires (N < 128)
			constexpr BaseN_Alphabet(
				const char (&text)[N],				      // null terminated string with le alphabet
				bool       is_case_sensitive = (N <= 33),  
				char       padding_character = '\0'
			) noexcept;

			constexpr char   from_int (int8_t x) const noexcept;
			constexpr int8_t from_char(char c)   const noexcept;
		};
	}
}

#include "base_n.inl"