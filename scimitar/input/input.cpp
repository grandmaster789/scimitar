#include "input.h"
#include "../util/algorithm.h"
#include "../core/logger.h"
#include "mouse.h"
#include "keyboard.h"

namespace scimitar::input {
    class Aggregator:
        public MessageHandler<Keyboard::OnKeyPressed>,
        public MessageHandler<Keyboard::OnKeyReleased>
    {
    public:
        Aggregator(Input* manager) {
            m_Mouse    = std::make_unique<Mouse>(manager);
            m_Keyboard = std::make_unique<Keyboard>(manager);
        }

        void operator()(const Keyboard::OnKeyPressed& kp) {
            if (kp.kbd != m_Keyboard.get())
                m_Keyboard->set_state(kp.key, true);
        }

        void operator()(const Keyboard::OnKeyReleased& kr) {
            if (kr.kbd != m_Keyboard.get())
                m_Keyboard->set_state(kr.key, false);
        }

        const Mouse* get_mouse() const {
            return m_Mouse.get();
        }

        const Keyboard* get_keyboard() const {
            return m_Keyboard.get();
        }

    private:
        std::unique_ptr<Mouse>    m_Mouse;
        std::unique_ptr<Keyboard> m_Keyboard;
    };
}

namespace scimitar {
    Input::Input(): 
        System("Input") 
    {
    }

    void Input::init() {
        System::init();

        m_Aggregator = std::make_unique<input::Aggregator>(this);
    }

    void Input::update() {
    }

    void Input::shutdown() {
        System::shutdown();

        m_Aggregator.reset();
    }

    void Input::register_device(Keyboard* kbd) {
        if (!util::contains(m_Keyboards, kbd))
            m_Keyboards.push_back(kbd);
        else
            gLog << "Duplicate keyboard registration, discarding...\n";
    }

    void Input::unregister_device(Keyboard* kbd) {
        auto it = util::find(m_Keyboards, kbd);

        if (it != std::end(m_Keyboards))
            m_Keyboards.erase(it);
        else
            gLog << "Cannot unregister unlisted keyboard, ignoring...";
    }

    void Input::register_device(Mouse* m) {
        if (!util::contains(m_Mice, m))
            m_Mice.push_back(m);
        else
            gLog << "Duplicate mouse registration, discarding...";
    }

    void Input::unregister_device(Mouse* m) {
        auto it = util::find(m_Mice, m);

        if (it != std::end(m_Mice))
            m_Mice.erase(it);
        else
            gLog << "Cannot unregister unlisted mouse, ignoring...";
    }

    const std::vector<Input::Keyboard*>& Input::get_keyboards() const {
        return m_Keyboards;
    }

    const std::vector<Input::Mouse*>& Input::get_mice() const {
        return m_Mice;
    }

    const Input::Keyboard* Input::get_keyboard() const {
        return m_Aggregator->get_keyboard();
    }

    const Input::Mouse* Input::get_mouse() const {
        return m_Aggregator->get_mouse();        
    }
}  // namespace scimitar