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
		m_Counter = 1000;
	}

	void update() override {
		--m_Counter;

		if (!m_Counter)
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