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
}  // namespace scimitar::input

namespace scimitar {
    class Input: 
        public core::System {
    public:
        using Keyboard = input::Keyboard;
        using Mouse    = input::Mouse;

        Input();

        void init()     override;
        void update()   override;
        void shutdown() override;

        void register_device  (Keyboard* kbd);
        void unregister_device(Keyboard* kbd);

        void register_device  (Mouse* m);
        void unregister_device(Mouse* m);

        const std::vector<Keyboard*>& get_keyboards() const;
        const std::vector<Mouse*>&    get_mice()      const;

    private:
        std::vector<Keyboard*> m_Keyboards;
        std::vector<Mouse*>    m_Mice;
    };
}  // namespace scimitar