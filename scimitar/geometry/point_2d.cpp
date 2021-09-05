#include "point_2d.h"

#include <ostream>
#include <format>

#include "../dependencies.h" // constexpr math functions...

namespace scimitar::geometry {
	constexpr Point2D::Point2D(float x, float y) noexcept :
		m_Coord{ x, y }
	{
	}

	constexpr Point2D::Point2D(const std::array<float, 2>& coord) noexcept :
		m_Coord(coord)
	{
	}

	constexpr float& Point2D::x() noexcept {
		return m_Coord[0];
	}

	constexpr const float& Point2D::x() const noexcept {
		return m_Coord[0];
	}

	constexpr float& Point2D::y() noexcept {
		return m_Coord[1];
	}

	constexpr const float& Point2D::y() const noexcept {
		return m_Coord[0];
	}

	constexpr Point2D& Point2D::operator += (const Direction2D& d) noexcept {
		m_Coord[0] += d.x();
		m_Coord[1] += d.y();

		return *this;
	}

	constexpr Point2D& Point2D::operator -= (const Direction2D& d) noexcept {
		m_Coord[0] -= d.x();
		m_Coord[1] -= d.y();

		return *this;
	}

	constexpr Point2D Point2D::operator + (const Direction2D& d) const noexcept {
		return Point2D(
			x() + d.x(),
			y() + d.y()
		);
	}
	
	constexpr Point2D Point2D::operator - (const Direction2D& d) const noexcept {
		return Point2D(
			x() - d.x(),
			y() - d.y()
		);
	}

	constexpr bool Point2D::operator == (const Point2D& p) const noexcept {
		return
			x() == p.x() &&
			y() == p.y();
	}

	constexpr bool Point2D::operator != (const Point2D& p) const noexcept {
		return !(*this == p);
	}

	constexpr Point2D midpoint(const Point2D& a, const Point2D& b) noexcept {
		return Point2D(
			(a.x() + b.x()) / 2.0f,
			(a.y() + b.y()) / 2.0f
		);
	}

	constexpr Point2D min(const Point2D& a, const Point2D& b) noexcept {
		return Point2D(
			std::min(a.x(), b.x()),
			std::min(a.y(), b.y())
		);
	}

	constexpr Point2D max(const Point2D& a, const Point2D& b) noexcept {
		return Point2D(
			std::max(a.x(), b.x()),
			std::max(a.y(), b.y())
		);
	}

	constexpr Point2D round(const Point2D& p) noexcept {
		return Point2D(
			gcem::round(p.x()),
			gcem::round(p.y())
		);
	}

	constexpr Point2D ceil(const Point2D& p) noexcept {
		return Point2D(
			gcem::ceil(p.x()),
			gcem::ceil(p.y())
		);
	}

	constexpr Point2D floor(const Point2D& p) noexcept {
		return Point2D(
			gcem::floor(p.x()),
			gcem::floor(p.y())
		);
	}

	std::string to_string(const Point2D& p) noexcept {
		return std::format("[{}, {}]", p.x(), p.y());
	}

	std::ostream& operator << (std::ostream& os, const Point2D& p) noexcept {
		os << to_string(p);
		return os;
	}
}