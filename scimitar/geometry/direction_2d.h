#pragma once

#include <array>
#include <iosfwd>
#include <string>

namespace scimitar::geometry {
	class Direction2D {
	public:
		constexpr Direction2D() noexcept = default;
		constexpr Direction2D(float x, float y) noexcept;
		constexpr Direction2D(const std::array<float, 2>& values) noexcept;

		// constexpr default copy + move
		constexpr Direction2D             (const Direction2D&) noexcept = default;
		constexpr Direction2D& operator = (const Direction2D&) noexcept = default;
		constexpr Direction2D             (Direction2D&&)      noexcept = default;
		constexpr Direction2D& operator = (Direction2D&&)      noexcept = default;

		constexpr       float& x()       noexcept;
		constexpr const float& x() const noexcept;
		constexpr       float& y()       noexcept;
		constexpr const float& y() const noexcept;

		friend std::string to_string(const Direction2D& d) noexcept;
		friend std::ostream& operator << (std::ostream& os, const Direction2D& d) noexcept;

	private:
		std::array<float, 2> m_Values = {};
	};
}