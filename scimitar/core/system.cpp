#include "system.h"
#include "../util/algorithm.h"

namespace scimitar::core {
	System::System(const std::string& unique_system_name):
		m_Name(unique_system_name)
	{
	}
	
	void System::init() {
		std::cout << "Starting subsystem " << get_name() << "\n";
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

	std::ostream& operator << (std::ostream& os, const System& s) {
		os << s.get_name() << "\n";
		return os;
	}
}