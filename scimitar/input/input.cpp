#include "input.h"
#include "../util/algorithm.h"

namespace scimitar {
    Input::Input(): 
        System("Input") 
    {
    }

    void Input::init() {
        System::init();
    }

    void Input::update() {
    }

    void Input::shutdown() {
        System::shutdown();
    }

    void Input::register_device(Keyboard* kbd) {
        if (!util::contains(m_Keyboards, kbd))
            m_Keyboards.push_back(kbd);
        else
            std::cout << "Duplicate keyboard registration, discarding...\n";
    }

    void Input::unregister_device(Keyboard* kbd) {
        auto it = util::find(m_Keyboards, kbd);

        if (it != std::end(m_Keyboards))
            m_Keyboards.erase(it);
        else
            std::cout << "Cannot unregister unlisted keyboard, ignoring...";
    }

    void Input::register_device(Mouse* m) {
        if (!util::contains(m_Mice, m))
            m_Mice.push_back(m);
        else
            std::cout << "Duplicate mouse registration, discarding...";
    }

    void Input::unregister_device(Mouse* m) {
        auto it = util::find(m_Mice, m);

        if (it != std::end(m_Mice))
            m_Mice.erase(it);
        else
            std::cout << "Cannot unregister unlisted mouse, ignoring...";
    }

    const std::vector<Input::Keyboard*>& Input::get_keyboards() const {
        return m_Keyboards;
    }

    const std::vector<Input::Mouse*>& Input::get_mice() const {
        return m_Mice;
    }
}  // namespace scimitar