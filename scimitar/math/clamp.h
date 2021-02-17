#pragma once

#include <cassert>

namespace scimitar::math {
	template <typename T>
	T clamp(
		T value, 
		T minimum, 
		T maximum
	) {
		assert(minimum <= maximum);

		if (value < minimum)
			return minimum;

		if (value > maximum)
			return maximum;

		return value;
	}
}