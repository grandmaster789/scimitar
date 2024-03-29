#include <iostream>
#include <chrono>
#include <thread>

#include <scimitar.h>

class TestApp :
	public scimitar::app::Application
{
public:
	TestApp() :
		Application("Scimitar test application")
	{
		register_setting("counter", &m_Counter);

		add_dependency("OS");
		add_dependency("Renderer");
	}

	void init() override {
	}

	void update() override {
		using namespace scimitar;
		using eKey = input::Keyboard::eKey;

		if (m_Engine->get<Input>()->get_keyboard()->is_down(eKey::escape))
			m_Engine->stop();
	}

	void shutdown() override {
		m_Counter = 0; // because this is persisted via settings we need to reset it during the 'cleanup' phase
	}

	int m_Counter = 0;
};

int main() {
	std::cout << "Launching application\n";

	{
		auto& engine = scimitar::core::Engine::instance();
		
		engine.set_application<TestApp>();
		engine.start();
	}
	
	std::cout << "Application closed\n";
}