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

        void registerDevice  (Keyboard* kbd);
        void unregisterDevice(Keyboard* kbd);

        void registerDevice  (Mouse* m);
        void unregisterDevice(Mouse* m);

        const std::vector<Keyboard*>& getKeyboards() const;
        const std::vector<Mouse*>&    getMice()      const;

    private:
        std::vector<Keyboard*> m_Keyboards;
        std::vector<Mouse*>    m_Mice;
    };
}  // namespace scimitar