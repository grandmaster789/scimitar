#pragma once

#include "../core/system.h"

#include <vector>

namespace scimitar::os {
    class Window;
}

namespace scimitar::input {
    class Keyboard;
    class Mouse;
    //class Gamepad;

    class Aggregator;
}  // namespace scimitar::input

namespace scimitar {
    class Input: 
        public core::System {
    public:
        using Keyboard   = input::Keyboard;
        using Mouse      = input::Mouse;
        using Aggregator = input::Aggregator;

        Input();

        void init()     override;
        void update()   override;
        void shutdown() override;

        void register_device  (Keyboard* kbd);
        void unregister_device(Keyboard* kbd);

        void register_device  (Mouse* m);
        void unregister_device(Mouse* m);

        // per-window virtual devices
        const std::vector<Keyboard*>& get_keyboards() const;
        const std::vector<Mouse*>&    get_mice()      const; // coordinates are relative to the window client area

        const Keyboard* get_keyboard() const; // aggregated virtual keyboard (for any window)
        const Mouse*    get_mouse() const;    // aggregated virtual mouse (coordinates are in the full virtual desktop space)

    private:
        std::vector<Keyboard*> m_Keyboards;
        std::vector<Mouse*>    m_Mice;

        std::unique_ptr<Aggregator> m_Aggregator;
    };
}  // namespace scimitar