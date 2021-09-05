#include "direction_2d.h"

#include <ostream>
#include <format>

namespace scimitar::geometry {
	constexpr Direction2D::Direction2D(float x, float y) noexcept:
		m_Values{ x, y }
	{
	}

	constexpr Direction2D::Direction2D(const std::array<float, 2>& values) noexcept:
		m_Values(values)
	{
	}

	constexpr float& Direction2D::x() noexcept {
		return m_Values[0];
	}
	constexpr const float& Direction2D::x() const noexcept {
		return m_Values[0];
	}

	constexpr float& Direction2D::y() noexcept {
		return m_Values[1];
	}

	constexpr const float& Direction2D::y() const noexcept {
		return m_Values[1];
	}

	std::string to_string(const Direction2D& d) noexcept {
		return std::format("<{}, {}>", d.x(), d.y());
	}

	std::ostream& operator << (std::ostream& os, const Direction2D& d) noexcept {
		os << to_string(d);

		return os;
	}
}