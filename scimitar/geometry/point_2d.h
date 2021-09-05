#pragma once

#include <array>
#include <iosfwd>
#include <string>

#include "direction_2d.h"

namespace scimitar::geometry {
	class Point2D {
	public:
		constexpr Point2D() noexcept = default;
		constexpr Point2D(float x, float y) noexcept;
		constexpr Point2D(const std::array<float, 2>& coord) noexcept;

		// constexpr copy+move
		constexpr Point2D             (const Point2D&) noexcept = default;
		constexpr Point2D& operator = (const Point2D&) noexcept = default;
		constexpr Point2D             (Point2D&&)      noexcept = default;
		constexpr Point2D& operator = (Point2D&&)      noexcept = default;

		constexpr       float& x()       noexcept;
		constexpr const float& x() const noexcept;
		constexpr       float& y()       noexcept;
		constexpr const float& y() const noexcept;

		constexpr Point2D& operator += (const Direction2D& d)       noexcept;
		constexpr Point2D& operator -= (const Direction2D& d)       noexcept;
		constexpr Point2D  operator +  (const Direction2D& d) const noexcept;
		constexpr Point2D  operator -  (const Direction2D& d) const noexcept;

		constexpr bool operator == (const Point2D& p) const noexcept;
		constexpr bool operator != (const Point2D& p) const noexcept;

		constexpr friend Point2D midpoint(const Point2D& a, const Point2D& b) noexcept;
		constexpr friend Point2D min     (const Point2D& a, const Point2D& b) noexcept;
		constexpr friend Point2D max     (const Point2D& a, const Point2D& b) noexcept;

		constexpr friend Point2D round(const Point2D& p) noexcept; // towards nearest integer
		constexpr friend Point2D ceil (const Point2D& p) noexcept;
		constexpr friend Point2D floor(const Point2D& p) noexcept;

		friend std::string to_string(const Point2D& p) noexcept;
		friend std::ostream& operator << (std::ostream& os, const Point2D& p) noexcept;

	private:
		std::array<float, 2> m_Coord = {}; // cartesian representation
	};
}