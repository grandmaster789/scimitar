#include "system.h"
#include "engine.h"
#include "../util/algorithm.h"
#include "../util/threads.h"

namespace scimitar::core {
	System::System(const std::string& unique_system_name):
		m_Name(unique_system_name)
	{
	}
	
	void System::init() {
		std::cout << "Starting subsystem " << get_name() << "\n";

		if (m_DedicatedThread)
			util::set_thread_name(get_name());
	}

	void System::update() {
	}

	void System::shutdown() {
		std::cout << "Stopped subsystem " << get_name() << "\n";
	}

	const std::string& System::get_name() const {
		return m_Name;
	}

	const System::Dependencies& System::get_dependencies() const {
		return m_Dependencies;
	}

	const System::Settings& System::get_settings() const {
		return m_Settings;
	}

	void System::add_dependency(std::string_view system_name) {
		if (!util::contains(m_Dependencies, system_name))
			m_Dependencies.push_back(std::string(system_name));
		else
			std::cout << "Ignoring duplicate dependency: " << system_name << "\n";
	}

	void System::operator()(const RequestShutdown& req) {
		if (req.m_System == this) {
			shutdown();
			broadcast(DedicatedThreadSync{});
		}
	}

	std::ostream& operator << (std::ostream& os, const System& s) {
		os << s.get_name() << "\n";
		return os;
	}
}