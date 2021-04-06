#include "base_n.h"
#include <cassert>
#include <bit>

namespace scimitar::util::codec::detail {
	constexpr char BaseN_Alphabet::from_int(int8_t x) const noexcept {
		assert(x < m_Radix);

		return m_IntToCharacter[x];
	}

	constexpr int8_t BaseN_Alphabet::from_char(char c) const noexcept {
		// was getting weird warnings C28020 here?
		//return m_CharacterToInt[c];
		return m_CharacterToInt[std::bit_cast<uint8_t>(c)];
	}
}