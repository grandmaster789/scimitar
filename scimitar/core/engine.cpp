#include "engine.h"
#include "../util/algorithm.h"
#include "../renderer/renderer.h"
#include "../dependencies.h"

#include <stdexcept>
#include <iostream>
#include <fstream>

namespace {
	void glfw_error_callback(int error_code, const char* description) {
		std::cerr << "GLFW error (" << error_code << "): " << description << "\n";
	}

	bool is_satisfied(
		scimitar::core::System*         s,
		const std::vector<std::string>& already_initialized
	) {
		using scimitar::util::contains;

		for (const auto& dependency : s->get_dependencies())
			if (!contains(already_initialized, dependency))
				return false;

		return true;
	}
}

namespace scimitar::core {
	Engine::Engine() {
		add<Renderer>();
	}

	Engine::~Engine() {
	}

	Engine& Engine::instance() {
		static Engine e;
		return e;
	}

	void Engine::start() {
		start_libraries();
		start_systems();

		m_Running = true;

		while (m_Running) {
			for (auto& system : m_Systems)
				system->update();

			if (m_Application)
				m_Application->update();
		}

		stop_systems();
		stop_libraries();
	}

	void Engine::stop() {
		m_Running = false;
	}

	void Engine::start_libraries() {
		// there are some flags that could be set as initialization hints for GLFW
		// (see https://www.glfw.org/docs/latest/intro_guide.html#init_hints_values)

		// glfw error callback should eventually also log the error
		glfwSetErrorCallback(&glfw_error_callback);

		if (glfwInit() != GLFW_TRUE)
			throw std::runtime_error("Failed to initialize GLFW");
	}

	void Engine::stop_libraries() {
		glfwTerminate(); // destroys all remaining windows etc		
	}

	void Engine::start_systems() {
		// first try and load subsystem settings
		{
			using namespace nlohmann;
			using namespace std;

			ifstream in("scimitar.json");
			if (in.good()) {
				json settings;
				in >> settings;

				for (const auto& ptr : m_Systems) {
					auto it = settings.find(ptr->get_name());

					if (it != settings.end()) {
						for (const auto& entry : ptr->get_settings())
							entry.m_SetFn(*it);
					}
					else
						std::cout << "No settings for subsystem " << ptr->get_name() << "\n";
				}

				if (m_Application) {
					auto it = settings.find(m_Application->get_name());

					if (it != settings.end()) {
						for (const auto& entry : m_Application->get_settings())
							entry.m_SetFn(*it);
					}
					else
						std::cout << "No application settings for " << m_Application->get_name() << "\n";
				}
			}
			else
				std::cout << "No configuration found, using defaults for all systems\n";
		}

		// start subsystem initialization; keep track of (satisfied) dependencies and store the order
		{
			size_t current_pass_count      = 0; // number of systems initialized at the beginning of a pass
			size_t num_systems_initialized = 0;
			size_t num_systems             = m_Systems.size();

			auto it = std::begin(m_Systems);

			while (true) {
				      auto* current_system = it->get();
				const auto& sytem_name     = current_system->get_name();

				bool already_initialized = util::contains(m_InitOrder, sytem_name);

				if (
					(!already_initialized) &&
					(is_satisfied(current_system, m_InitOrder))
				) {
					current_system->m_Engine = this;
					current_system->init();

					m_InitOrder.push_back(sytem_name);
					++num_systems_initialized;
				}

				++it;

				// if we reached the end, see if we're actually done (or that we've stalled)
				if (it == std::end(m_Systems)) {
					it = std::begin(m_Systems);

					if (num_systems_initialized == current_pass_count) {
						// we've stalled, figure out which systems failed to initialize
						std::cout << "Stalled during system initialization\n";

						for (const auto& ptr : m_Systems)
							if (!util::contains(m_InitOrder, sytem_name))
								std::cout << "\tFailed to initialize: " << sytem_name << "\n";

						throw std::runtime_error("Stalled during system initialization\n");
					}

					if (num_systems_initialized == num_systems)
						break; // all systems were initialized

					current_pass_count = num_systems_initialized;
				}
			}
		}

		// finally, start the application
		if (m_Application) {
			m_Application->m_Engine = this;

			if (is_satisfied(m_Application.get(), m_InitOrder))
				m_Application->init();
			else
				throw std::runtime_error("Application dependencies were not satisfied");
		}
		else
			std::cout << "No application was set\n"; // perhaps some kind of diagnosic mode?
	}

	void Engine::stop_systems() {
		if (m_Application)
			m_Application->shutdown();

		for (auto it = std::rbegin(m_InitOrder); it != std::rend(m_InitOrder); ++it) {
			auto jt = util::find_if(
				m_Systems, 
				[name = *it](const SystemPtr& ptr) {
					return ptr->get_name() == name;
				}
			);

			if (jt == std::end(m_Systems))
				// somehow the system was initialized and later removed
				std::cout << "Cannot shutdown system: " << *it << "\n";
			else
				(*jt)->shutdown();
		}

		// traverse and consolidate settings from all systems and the current application
		{
			using namespace nlohmann;
			
			json settings;

			if (m_Application) {
				json js;

				for (const auto& entry : m_Application->get_settings()) {
					auto fetch = entry.m_GetFn();
					
					if (fetch.size() != 1)
						throw std::runtime_error("Currently only singular variables are supported");

					js.emplace(
						 fetch.begin().key(),
						*fetch.begin()
					);
				}

				settings[m_Application->get_name()] = js;
			}

			// now the rest of the subsystems
			for (const auto& system : m_Systems) {
				json js;

				for (const auto& entry : system->get_settings()) {
					auto fetch = entry.m_GetFn();

					if (fetch.size() != 1)
						throw std::runtime_error("Currently only singular variables are supported");

					js.emplace(
						 fetch.begin().key(),
						*fetch.begin()
					);
				}

				settings[system->get_name()] = js;
			}

			std::ofstream out("scimitar.json");
			if (!out.good())
				std::cout << "Failed to write to scimitar.json\n";
			else
				out << settings.dump(2);
		}
	}
}