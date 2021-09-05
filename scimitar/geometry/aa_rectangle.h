#pragma once

#include "../dependencies.h"
#include "extent_2d.h"
#include "point_2d.h"

namespace scimitar::geometry {
	
	class AxisAlignedRectangle {
	public:
		constexpr AxisAlignedRectangle() noexcept = default;
		constexpr AxisAlignedRectangle(
			float left,
			float bottom,
			float right,
			float top
		) noexcept;
		constexpr AxisAlignedRectangle(const Extent2D& e) noexcept;
		constexpr AxisAlignedRectangle(const Point2D& bottom_left, const Point2D& top_right) noexcept;
		constexpr AxisAlignedRectangle(const Point2D& bottom_left, const Extent2D& size) noexcept;
				  
		// (constexpr) defaulted copy+move
		constexpr AxisAlignedRectangle             (const AxisAlignedRectangle&) noexcept = default;
		constexpr AxisAlignedRectangle& operator = (const AxisAlignedRectangle&) noexcept = default;
		constexpr AxisAlignedRectangle             (AxisAlignedRectangle&&)      noexcept = default;
		constexpr AxisAlignedRectangle& operator = (AxisAlignedRectangle&&)      noexcept = default;

		constexpr bool is_valid() const noexcept;
		constexpr bool is_empty() const noexcept;

		constexpr operator bool() const noexcept; // non-empty

		/*
		*   Corners (valid indices 0-3) are retrieved in CCW order:
		*
		*   3--2
		*   |  |
		*   |  |
		*   0--1
		*/
		constexpr Point2D operator[](size_t idx) const noexcept;
	
		constexpr bool operator == (const AxisAlignedRectangle& rect) const noexcept;
		constexpr bool operator != (const AxisAlignedRectangle& rect) const noexcept;

		constexpr Extent2D extent() const noexcept;
		constexpr float    width()  const noexcept;
		constexpr float    height() const noexcept;

		constexpr AxisAlignedRectangle& set_width (float w) noexcept; // bottom-left remains the same
		constexpr AxisAlignedRectangle& set_height(float h) noexcept; // bottom-left remains the same
		constexpr AxisAlignedRectangle& set_scale (float s) noexcept; // center remains the same

		constexpr float left()   const noexcept;
		constexpr float bottom() const noexcept;
		constexpr float right()  const noexcept;
		constexpr float top()    const noexcept;

		constexpr float   horizontal_middle() const noexcept;
		constexpr float   vertical_middle()   const noexcept;
		constexpr Point2D get_center()        const noexcept;

		constexpr AxisAlignedRectangle& operator |= (const AxisAlignedRectangle& rect)       noexcept; // rectangle that encompasses both
		constexpr AxisAlignedRectangle  operator |  (const AxisAlignedRectangle& rect) const noexcept;
		constexpr AxisAlignedRectangle& operator |= (const Point2D& point)       noexcept;
		constexpr AxisAlignedRectangle  operator |  (const Point2D& point) const noexcept;

		constexpr bool contains(const Point2D& point)             const noexcept;
		constexpr bool overlaps(const AxisAlignedRectangle& rect) const noexcept;

		friend constexpr AxisAlignedRectangle intersect(const AxisAlignedRectangle& a, const AxisAlignedRectangle& b) noexcept;

		friend constexpr AxisAlignedRectangle scale(const AxisAlignedRectangle& rect, float s) noexcept;
		friend constexpr AxisAlignedRectangle expand(const AxisAlignedRectangle& rect, float amount) noexcept;
		friend constexpr AxisAlignedRectangle shrink(const AxisAlignedRectangle& rect, float amount) noexcept;

		friend constexpr AxisAlignedRectangle round(const AxisAlignedRectangle& rect) noexcept;
		friend constexpr AxisAlignedRectangle ceil (const AxisAlignedRectangle& rect) noexcept;
		friend constexpr AxisAlignedRectangle floor(const AxisAlignedRectangle& rect) noexcept;

	private:
		float m_Left   = 0.0f;
		float m_Bottom = 0.0f;
		float m_Right  = 0.0f;
		float m_Top    = 0.0f;
	};
}