#include "system.h"
#include "../util/algorithm.h"

namespace scimitar::core {
	System::System(const std::string& unique_system_name) {
	}
	
	void System::start() {
		std::cout << "Starting " << getName() << "\n";
	}

	void System::update() {
	}

	void System::stop() {
		std::cout << "Stopped " << getName() << "\n";
	}

	std::string_view System::getName() const {
		return m_Name;
	}

	const System::Dependencies& System::getDependencies() const {
		return m_Dependencies;
	}

	const System::Settings& System::getSettings() const {
		return m_Settings;
	}

	void System::add_dependency(std::string_view system_name) {
		if (!util::contains(m_Dependencies, system_name))
			m_Dependencies.push_back(std::string(system_name));
		else
			std::cout << "Ignoring duplicate dependency: " << system_name << "\n";
	}

	std::ostream& operator << (std::ostream& os, const System& s) {
		return os;
	}
}