#pragma once

#include "../algorithm.h"

namespace scimitar::util::codec {
	namespace detail {
		template <size_t N>
		requires(N < 128)
		constexpr BaseN_Alphabet::BaseN_Alphabet(
			const char(&text)[N],				      // null terminated string with le alphabet
			bool       is_case_sensitive,
			char       padding_character
		) noexcept :
			m_Radix           (N - 1),
			m_CaseSensitive   (is_case_sensitive),
			m_PaddingCharacter(padding_character)
		{
			// start everything at -2
			fill(m_CharacterToInt, -2);

			// whitespace should be marked -1
			m_CharacterToInt[' ']  = -1;
			m_CharacterToInt['\t'] = -1;
			m_CharacterToInt['\r'] = -1;
			m_CharacterToInt['\n'] = -1;
			m_CharacterToInt['\f'] = -1;

			if (padding_character)
				m_CharacterToInt[padding_character] = -1;

			// actually set up the table
			for (size_t i = 0; i < m_Radix; ++i) {
				char c = text[i];

				m_IntToCharacter[i] = c;
				m_CharacterToInt[c] = static_cast<int8_t>(i);

				// consider case insensitivity
				if (!is_case_sensitive) {
					if (c >= 'a' && c <= 'z')
						m_CharacterToInt[(c - 'a') + 'A'] = static_cast<int8_t>(i);
					else if (c >= 'A' && c <= 'Z')
						m_CharacterToInt[(c - 'A') + 'a'] = static_cast<int8_t>(i);
				}
			}
		}
	}
}