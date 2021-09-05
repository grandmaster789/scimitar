#include "aa_rectangle.h"
#include <cassert>

namespace scimitar::geometry {
	constexpr AxisAlignedRectangle::AxisAlignedRectangle(
		float left,
		float bottom,
		float right,
		float top
	) noexcept:
		m_Left  (left),
		m_Bottom(bottom),
		m_Right (right),
		m_Top   (top)
	{
	}

	constexpr AxisAlignedRectangle::AxisAlignedRectangle(const Extent2D& e) noexcept:
		m_Left  (0.0f),
		m_Bottom(0.0f),
		m_Right (e.width()),
		m_Top   (e.height())
	{
	}

	constexpr AxisAlignedRectangle::AxisAlignedRectangle(
		const Point2D& bottom_left,
		const Point2D& top_right
	) noexcept :
		m_Left  (bottom_left.x()),
		m_Bottom(bottom_left.y()),
		m_Right (top_right.x()),
		m_Top   (top_right.y())
	{
	}

	constexpr AxisAlignedRectangle::AxisAlignedRectangle(
		const Point2D& bottom_left,
		const Extent2D& size
	) noexcept:
		m_Left  (bottom_left.x()),
		m_Bottom(bottom_left.y()),
		m_Right (bottom_left.x() + size.width()),
		m_Top   (bottom_left.y() + size.height())
	{
	}

	constexpr bool AxisAlignedRectangle::is_valid() const noexcept {
		return
			(m_Left   <= m_Right) &&
			(m_Bottom <= m_Top);
	}

	constexpr bool AxisAlignedRectangle::is_empty() const noexcept {
		return 
			(m_Left == m_Right) &&
			(m_Top  == m_Bottom);
	}

	constexpr AxisAlignedRectangle::operator bool() const noexcept {
		return !is_empty();
	}

	constexpr Point2D AxisAlignedRectangle::operator[](size_t idx) const noexcept {
		switch (idx) {
		case 0: return Point2D(m_Left,  m_Bottom);
		case 1: return Point2D(m_Right, m_Bottom);
		case 2: return Point2D(m_Right, m_Top);
		case 3: return Point2D(m_Left,  m_Top);

		default:
			assert(false);
			return Point2D();
		}
	}

	constexpr bool AxisAlignedRectangle::operator == (const AxisAlignedRectangle& rect) const noexcept {
		return
			(m_Left   == rect.left())   &&
			(m_Bottom == rect.bottom()) &&
			(m_Right  == rect.right())  &&
			(m_Top    == rect.top());
	}

	constexpr bool AxisAlignedRectangle::operator != (const AxisAlignedRectangle& rect) const noexcept {
		return !(*this == rect);
	}

	constexpr Extent2D AxisAlignedRectangle::extent() const noexcept {
		return Extent2D(width(), height());
	}

	constexpr float AxisAlignedRectangle::width() const noexcept {
		return m_Right - m_Left;
	}

	constexpr float AxisAlignedRectangle::height() const noexcept {
		return m_Top - m_Bottom;
	}

	constexpr AxisAlignedRectangle& AxisAlignedRectangle::set_width(float w) noexcept {
		m_Right = m_Left + w;
		return *this;
	}

	constexpr AxisAlignedRectangle& AxisAlignedRectangle::set_height(float h) noexcept {
		m_Top = m_Bottom + h;
		return *this;
	}

	constexpr AxisAlignedRectangle& AxisAlignedRectangle::set_scale(float s) noexcept {
		auto new_extent_half = extent() * s / 2.0f;
		auto center          = get_center();

		m_Left   = center.x() - new_extent_half.width();
		m_Right  = center.x() + new_extent_half.width();
		m_Bottom = center.y() - new_extent_half.height();
		m_Top    = center.y() + new_extent_half.height();

		return *this;
	}

	constexpr float AxisAlignedRectangle::left() const noexcept {
		return m_Left;
	}

	constexpr float AxisAlignedRectangle::bottom() const noexcept {
		return m_Bottom;
	}

	constexpr float AxisAlignedRectangle::right() const noexcept {
		return m_Right;
	}

	constexpr float AxisAlignedRectangle::top() const noexcept {
		return m_Top;
	}

	constexpr float AxisAlignedRectangle::horizontal_middle() const noexcept {
		return (m_Left + m_Right) / 2.0f;
	}

	constexpr float AxisAlignedRectangle::vertical_middle() const noexcept {
		return (m_Bottom + m_Top) / 2.0f;
	}

	constexpr Point2D AxisAlignedRectangle::get_center() const noexcept {
		return Point2D(
			horizontal_middle(),
			vertical_middle()
		);
	}

	constexpr AxisAlignedRectangle& AxisAlignedRectangle::operator |= (const AxisAlignedRectangle& rect) noexcept {
		if (is_empty()) {
			*this = rect;
			return *this;
		}

		if (rect.is_empty())
			return *this;

		m_Left   = std::min(m_Left,   rect.left());
		m_Bottom = std::min(m_Bottom, rect.bottom());
		m_Right  = std::max(m_Right,  rect.right());
		m_Top    = std::max(m_Top,    rect.top());

		return *this;
	}

	constexpr AxisAlignedRectangle AxisAlignedRectangle::operator | (const AxisAlignedRectangle& rect) const noexcept {
		if (is_empty())
			return rect;

		if (rect.is_empty())
			return *this;

		return AxisAlignedRectangle(
			std::min(m_Left,   rect.left()),
			std::min(m_Bottom, rect.bottom()),
			std::max(m_Right,  rect.right()),
			std::max(m_Top,    rect.top())
		);
	}

	constexpr AxisAlignedRectangle& AxisAlignedRectangle::operator |= (const Point2D& point) noexcept {
		m_Left   = std::min(m_Left,   point.x());
		m_Bottom = std::min(m_Bottom, point.y());
		m_Right  = std::max(m_Right,  point.x());
		m_Top    = std::max(m_Top,    point.y());

		return *this;
	}

	constexpr AxisAlignedRectangle  AxisAlignedRectangle::operator |  (const Point2D& point) const noexcept {
		return AxisAlignedRectangle(
			std::min(m_Left,   point.x()),
			std::min(m_Bottom, point.y()),
			std::max(m_Right,  point.x()),
			std::max(m_Top,    point.y())
		);
	}

	constexpr bool AxisAlignedRectangle::contains(const Point2D& point) const noexcept {
		return
			(point.x() >= m_Left) &&
			(point.x() <  m_Right) &&
			(point.y() >= m_Bottom) &&
			(point.y() <  m_Top);
	}

	constexpr bool AxisAlignedRectangle::overlaps(const AxisAlignedRectangle& rect) const noexcept {
		assert(is_valid() && rect.is_valid());

		if (is_empty() || rect.is_empty())
			return false;

		if (
			(m_Left   > rect.right()) ||
			(m_Bottom > rect.top())   ||
			(m_Right  < rect.left())  ||
			(m_Top    < rect.bottom())
		)
			return false;

		return true;
	}

	constexpr AxisAlignedRectangle intersect(
		const AxisAlignedRectangle& a,
		const AxisAlignedRectangle& b
	) noexcept {
		auto bottom_left = max(a[0], b[0]);
		auto top_right   = min(a[2], b[2]);

		// if this yields a valid rectangle, return that
		if (
			(bottom_left.x() < top_right.x()) &&
			(bottom_left.y() < top_right.y())
			)
			return AxisAlignedRectangle(bottom_left, top_right);
		else
			return AxisAlignedRectangle(); // otherwise yield an empty/zeroed one
	}

	constexpr AxisAlignedRectangle scale(const AxisAlignedRectangle& rect, float s) noexcept {
		return
			AxisAlignedRectangle(rect).set_scale(s);
	}

	constexpr AxisAlignedRectangle expand(const AxisAlignedRectangle& rect, float amount) noexcept {
		return AxisAlignedRectangle(
			rect.left()   - amount,
			rect.bottom() - amount,
			rect.right()  + amount,
			rect.top()    + amount
		);
	}

	constexpr AxisAlignedRectangle shrink(const AxisAlignedRectangle& rect, float amount) noexcept {
		return expand(rect, -amount);
	}

	constexpr AxisAlignedRectangle round(const AxisAlignedRectangle& rect) noexcept {
		auto bottom_left = round(rect[0]);
		auto top_right   = round(rect[2]);

		return AxisAlignedRectangle(bottom_left, top_right);
	}

	constexpr AxisAlignedRectangle ceil(const AxisAlignedRectangle& rect) noexcept {
		auto bottom_left = ceil(rect[0]);
		auto top_right   = ceil(rect[2]);

		return AxisAlignedRectangle(bottom_left, top_right);
	}

	constexpr AxisAlignedRectangle floor(const AxisAlignedRectangle& rect) noexcept {
		auto bottom_left = floor(rect[0]);
		auto top_right   = floor(rect[2]);

		return AxisAlignedRectangle(bottom_left, top_right);
	}
}