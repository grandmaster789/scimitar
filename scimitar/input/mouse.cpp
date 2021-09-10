#include "mouse.h"
#include "input.h"
#include "../core/mediator.h"

#include <ostream>
#include <format>

namespace scimitar::input {
    Mouse::Mouse(Input* manager):
        m_Manager(manager)
    {
        m_Manager->register_device(this);
    }

    Mouse::~Mouse() {
        m_Manager->unregister_device(this);
    }

    bool Mouse::is_down(eButton button) const {
        switch (button) {
        case eButton::left:   return m_Buttons[0];
        case eButton::middle: return m_Buttons[1];
        case eButton::right:  return m_Buttons[2];
        default:
            throw std::runtime_error("Unsupported mouse button");
        }
    }

    bool Mouse::is_up(eButton button) const {
        switch (button) {
        case eButton::left:   return !m_Buttons[0];
        case eButton::middle: return !m_Buttons[1];
        case eButton::right:  return !m_Buttons[2];
        default:
            throw std::runtime_error("Unsupported mouse button");
        }
    }

    std::pair<float, float> Mouse::get_position() const {
        return { m_X, m_Y };
    }

    void Mouse::set_state(eButton button, bool pressed) {
        int idx = 0;

        switch (button) {
        case eButton::left:   idx = 0; break;
        case eButton::middle: idx = 1; break;
        case eButton::right:  idx = 2; break;
        default:
            throw std::runtime_error("Unsupported button");
        }

        if (m_Buttons[idx] != pressed) {
            m_Buttons[idx] = pressed;

            if (pressed)
                broadcast(OnButtonPressed{ this, m_X, m_Y, button });
            else
                broadcast(OnButtonReleased{ this, m_X, m_Y, button });
        }
    }

    void Mouse::set_position(float x, float y) {
        if (
            (m_X != x) ||
            (m_Y != y)
        ) {
            auto dx = m_X - x;
            auto dy = m_Y - y;

            m_X = x;
            m_Y = y;

            broadcast(OnMoved{ this, x, y, dx, dy });
        }
    }

    void Mouse::do_double_click(eButton button) {
        broadcast(OnDoubleClick{ this, m_X, m_Y, button });
    }

    void Mouse::do_scroll(int amount) {
        broadcast(OnScroll{ this, amount });
    }

    void Mouse::do_enter(Window* w) {
        broadcast(OnEnterWindow{ this, w });
    }

    void Mouse::do_leave(Window* w) {
        broadcast(OnLeaveWindow{ this, w });
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::eButton& button) {
        switch (button) {
        case Mouse::eButton::left:   os << "Left";   break;
        case Mouse::eButton::middle: os << "Middle"; break;
        case Mouse::eButton::right:  os << "Right";  break;

        default:
            os << "[unknown]"; 
            break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnMoved& mm) {
        os << std::format("Mouse moved: [{},{}] - [{}, {}]", mm.m_X, mm.m_Y, mm.m_DeltaX, mm.m_DeltaY);

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnButtonPressed& bp) {
        os 
            << "Mouse button pressed: " << bp.m_Button 
            << " @ [" << bp.m_X << ", " << bp.m_Y << "]";

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnButtonReleased& br) {
        os
            << "Mouse button released: " << br.m_Button
            << " @ [" << br.m_X << ", " << br.m_Y << "]";

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnDoubleClick& bc) {
        os
            << "Mouse double click: " << bc.m_Button
            << " @ [" << bc.m_X << ", " << bc.m_Y << "]";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnScroll& ms) {
        os << "Mouse scroll: " << ms.m_ScrollAmount;

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnEnterWindow& ew) {
        (void)ew;
        os << "Mouse entered window";

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Mouse::OnLeaveWindow& lw) {
        (void)lw;
        os << "Mouse left window";

        return os;
    }

    std::ostream& operator << (std::ostream& os, const Mouse& m) {
        auto pos = m.get_position();

        os << "Mouse [" << pos.first << ", " << pos.second << "] ";

        return os;
    }
}