#include "extent_2d.h"

#include "../math/helpers.h"
#include "../dependencies.h"

#include <numeric>
#include <ostream>
#include <tuple>
#include <format>
#include <cassert>

namespace scimitar::geometry {
	constexpr Extent2D::Extent2D(
		float width,
		float height
	) noexcept :
		m_Width (width),
		m_Height(height)
	{
	}

	constexpr Extent2D Extent2D::infinite() noexcept {
		return Extent2D(
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity()
		);
	}

	constexpr Extent2D Extent2D::nan() noexcept {
		return Extent2D(
			std::numeric_limits<float>::signaling_NaN(),
			std::numeric_limits<float>::signaling_NaN()
		);
	}

	constexpr const float& Extent2D::width() const noexcept {
		return m_Width;
	}

	constexpr float& Extent2D::width() noexcept {
		return m_Width;
	}

	constexpr const float& Extent2D::height() const noexcept {
		return m_Height;
	}

	constexpr float& Extent2D::height() noexcept {
		return m_Height;
	}

	constexpr Extent2D::operator bool() const noexcept {
		return 
			(m_Width  != 0.0f) &&
			(m_Height != 0.0f);
	}

	constexpr bool Extent2D::is_valid() const noexcept {
		return
			(m_Width  != 0.0f) &&
			(m_Height != 0.0f);
	}

	constexpr Extent2D& Extent2D::operator += (const Extent2D& e) noexcept {
		m_Width  += e.width();
		m_Height += e.height();

		return *this;
	}

	constexpr Extent2D& Extent2D::operator -= (const Extent2D& e) noexcept {
		m_Width  -= e.width();
		m_Height -= e.height();

		return *this;
	}

	constexpr Extent2D  Extent2D::operator + (const Extent2D& e) const noexcept {
		return Extent2D(
			m_Width  + e.width(),
			m_Height + e.height()
		);
	}
	constexpr Extent2D  Extent2D::operator - (const Extent2D& e) const noexcept {
		return Extent2D(
			m_Width  - e.width(),
			m_Height - e.height()
		);
	}

	constexpr Extent2D& Extent2D::operator *= (float scalar) noexcept {
		m_Width  *= scalar;
		m_Height *= scalar;

		return *this;
	}

	constexpr Extent2D& Extent2D::operator /= (float scalar) noexcept {
		m_Width  /= scalar;
		m_Height /= scalar;

		return *this;
	}

	constexpr Extent2D Extent2D::operator * (float scalar) const noexcept {
		return Extent2D(
			m_Width  * scalar,
			m_Height * scalar
		);
	}

	constexpr Extent2D Extent2D::operator / (float scalar) const noexcept {
		return Extent2D(
			m_Width  / scalar,
			m_Height / scalar
		);
	}

	constexpr Extent2D operator * (float scalar, const Extent2D& e) noexcept {
		return Extent2D(
			scalar * e.width(),
			scalar * e.height()
		);
	}

	constexpr bool Extent2D::operator == (const Extent2D& e) const noexcept {
		return
			(m_Width  == e.m_Width) &&
			(m_Height == e.m_Height);
	}

	constexpr bool Extent2D::operator != (const Extent2D& e) const noexcept {
		return !(*this == e);
	}

	constexpr bool Extent2D::operator < (const Extent2D& e) const noexcept {
		return 
			std::tie(  m_Width,   m_Height) < 
			std::tie(e.m_Width, e.m_Height);
	}

	constexpr bool Extent2D::operator > (const Extent2D& e) const noexcept {
		return
			std::tie(  m_Width,   m_Height) >
			std::tie(e.m_Width, e.m_Height);
	}

	constexpr bool Extent2D::operator <= (const Extent2D& e) const noexcept {
		return
			std::tie(  m_Width,   m_Height) <=
			std::tie(e.m_Width, e.m_Height);
	}

	constexpr bool Extent2D::operator >= (const Extent2D& e) const noexcept {
		return
			std::tie(  m_Width,   m_Height) >=
			std::tie(e.m_Width, e.m_Height);
	}

	constexpr float sq_hypot(const Extent2D& e) noexcept {
		return
			math::square(e.width()) +
			math::square(e.height());
	}

	constexpr float hypot(const Extent2D& e) noexcept {
		return gcem::sqrt(sq_hypot(e));
	}

	constexpr Extent2D normalize(const Extent2D& e) noexcept {
		assert(e.is_valid());
		return e / hypot(e);
	}

	constexpr Extent2D min(const Extent2D& a, const Extent2D& b) noexcept {
		return Extent2D(
			std::min(a.width(),  b.width()),
			std::min(a.height(), b.height())
		);
	}

	constexpr Extent2D max(const Extent2D& a, const Extent2D& b) noexcept {
		return Extent2D(
			std::max(a.width(),  b.width()),
			std::max(a.height(), b.height())
		);
	}

	constexpr Extent2D clamp(
		const Extent2D& e,
		const Extent2D& minimum,
		const Extent2D& maximum
	) noexcept {
		return Extent2D(
			std::clamp(e.width(),  minimum.width(),  maximum.width()),
			std::clamp(e.height(), minimum.height(), maximum.height())
		);
	}

	std::string to_string(const Extent2D& e) noexcept {
		return std::format(
			"[{}, {}]",
			e.width(),
			e.height()
		);
	}

	std::ostream& operator << (std::ostream& os, const Extent2D& e) noexcept {
		os << to_string(e);

		return os;
	}
}