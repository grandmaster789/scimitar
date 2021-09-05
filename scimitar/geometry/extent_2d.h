#pragma once

#include <string>
#include <iosfwd>

#include "../dependencies.h"

namespace scimitar::geometry {
	/*
	*	Axis-aligned 2d extents
	*/
	class Extent2D {
	public:
		constexpr Extent2D() noexcept = default;
		constexpr Extent2D(
			float width, 
			float height
		) noexcept;
		
		static constexpr Extent2D infinite() noexcept;
		static constexpr Extent2D nan() noexcept;

		constexpr Extent2D             (const Extent2D&) noexcept = default;
		constexpr Extent2D& operator = (const Extent2D&) noexcept = default;
		constexpr Extent2D             (Extent2D&&)      noexcept = default;
		constexpr Extent2D& operator = (Extent2D&&)      noexcept = default;

		constexpr const float& width()  const noexcept;
		constexpr       float& width()        noexcept;
		constexpr const float& height() const noexcept;
		constexpr       float& height()       noexcept;

		constexpr explicit operator bool() const noexcept;
		constexpr bool is_valid() const noexcept;

		constexpr Extent2D& operator += (const Extent2D& e)       noexcept;
		constexpr Extent2D& operator -= (const Extent2D& e)       noexcept;
		constexpr Extent2D  operator +  (const Extent2D& e) const noexcept;
		constexpr Extent2D  operator -  (const Extent2D& e) const noexcept;

		       constexpr Extent2D& operator *= (float scalar)       noexcept;
		       constexpr Extent2D& operator /= (float scalar)       noexcept;
		       constexpr Extent2D  operator *  (float scalar) const noexcept;
		       constexpr Extent2D  operator /  (float scalar) const noexcept;
		friend constexpr Extent2D  operator *  (float scalar, const Extent2D& e) noexcept;

		constexpr bool operator == (const Extent2D& e) const noexcept;
		constexpr bool operator != (const Extent2D& e) const noexcept;
		constexpr bool operator <  (const Extent2D& e) const noexcept;
		constexpr bool operator >  (const Extent2D& e) const noexcept;
		constexpr bool operator <= (const Extent2D& e) const noexcept;
		constexpr bool operator >= (const Extent2D& e) const noexcept;

		friend constexpr float sq_hypot (const Extent2D& e) noexcept;
		friend constexpr float hypot    (const Extent2D& e) noexcept; // NOTE std::sqrt is not constexpr so we're rolling our own...

		friend constexpr Extent2D normalize(const Extent2D& e) noexcept;
		friend constexpr Extent2D min(const Extent2D& a, const Extent2D& b) noexcept;
		friend constexpr Extent2D max(const Extent2D& a, const Extent2D& b) noexcept;
		friend constexpr Extent2D clamp(
			const Extent2D& e, 
			const Extent2D& minimum,
			const Extent2D& maximum
		) noexcept;

		friend std::string to_string(const Extent2D& e) noexcept;
		friend std::ostream& operator << (std::ostream& os, const Extent2D& e) noexcept;

	private:
		float m_Width  = 0.0f;
		float m_Height = 0.0f;
	};
}