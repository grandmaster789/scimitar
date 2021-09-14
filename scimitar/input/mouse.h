#pragma once

#include <iosfwd>
#include <utility>
#include <array>

namespace scimitar {
    class Input;

    namespace os {
        class Window;
    }

    namespace input {
        // [TODO] when I've got some math basics in place, the XY stuff should be some
        //        kind of point struct (plus directions perhaps?)
        // [TODO] do I want dragging support?

        // [NOTE] X/Y coordinates are normalized to [-1..1] within the associated window

        class Mouse {
        public:
            using Window = os::Window;

            enum class eButton
            {
                left,
                right,
                middle
            };

            Mouse(Input* manager);
            ~Mouse();

            Mouse             (const Mouse&) = delete;
            Mouse& operator = (const Mouse&) = delete;
            Mouse             (Mouse&&)      = delete;
            Mouse& operator = (Mouse&&)      = delete;

            bool is_down(eButton button) const;
            bool is_up(eButton button) const;

            std::pair<float, float> get_position() const;

            void set_state(eButton button, bool pressed);
            void set_position(float x, float y);

            void do_double_click(eButton button);
            void do_scroll(int amount);
            void do_enter(Window* w);
            void do_leave(Window* w);

            // --------------------- Events -----------------------
            struct OnMoved {
                Mouse* m_Mouse;
                float  m_X;
                float  m_Y;

                float m_DeltaX;
                float m_DeltaY;
            };

            struct OnButtonPressed {
                Mouse*  m_Mouse;
                float   m_X;
                float   m_Y;
                eButton m_Button;
            };

            struct OnButtonReleased {
                Mouse*  m_Mouse;
                float   m_X;
                float   m_Y;
                eButton m_Button;
            };

            struct OnDoubleClick {
                Mouse*  m_Mouse;
                float   m_X;
                float   m_Y;
                eButton m_Button;
            };

            struct OnScroll {
                Mouse* m_Mouse;
                int    m_ScrollAmount;
            };

            struct OnEnterWindow {
                Mouse*  m_Mouse;
                Window* m_Window;
            };

            struct OnLeaveWindow {
                Mouse*  m_Mouse;
                Window* m_Window;
            };

        private:
            Input* m_Manager = nullptr;

            std::array<bool, 3> m_Buttons = {};

            float m_X = 0;
            float m_Y = 0;
        };

        std::ostream& operator << (std::ostream& os, const Mouse::eButton& button);
                                  
        std::ostream& operator << (std::ostream& os, const Mouse::OnMoved& mm);
        std::ostream& operator << (std::ostream& os, const Mouse::OnButtonPressed& bp);
        std::ostream& operator << (std::ostream& os, const Mouse::OnButtonReleased& br);
        std::ostream& operator << (std::ostream& os, const Mouse::OnDoubleClick& bc);
        std::ostream& operator << (std::ostream& os, const Mouse::OnScroll& ms);
        std::ostream& operator << (std::ostream& os, const Mouse::OnEnterWindow& ew);
        std::ostream& operator << (std::ostream& os, const Mouse::OnLeaveWindow& lw);

        std::ostream& operator << (std::ostream& os, const Mouse& m);
    }  // namespace input
}  // namespace scimitar