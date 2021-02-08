#include "engine.h"
#include "../util/algorithm.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

namespace {
	void glfw_error_callback(int error_code, const char* description) {
		std::cerr << "GLFW error (" << error_code << "): " << description << "\n";
	}
}

namespace scimitar::core {
	Engine::Engine() {
		try {
			initLibraries();
		}
		catch (std::exception& ex) {
			std::cerr << "Exception during initialization: " << ex.what() << "\n";
		}
	}

	Engine::~Engine() {
	}

	Engine& Engine::instance() {
		static Engine e;
		return e;
	}
	
	os::Window* Engine::createWindow(
		const char* title, 
		int         width,
		int         height
	) {
		auto win = std::make_unique<os::Window>(title, width, height);

		auto result = win.get();

		m_Windows.push_back(std::move(win));

		return result;
	}

	void Engine::start() {
		while (!m_Windows.empty()) {
			auto local_copy = util::create_non_owning_copy(m_Windows);

			for (auto* w : local_copy)
				if (w->should_close())
					std::erase_if(
						m_Windows,
						[w](const auto& entry) { 
							return entry.get() == w; 
						}
					);

			glfwPollEvents();
		}

		closeLibraries();
	}

	void Engine::stop() {
		throw std::runtime_error("not implemented");
	}

	void Engine::initLibraries() {
		// there are some flags that could be set as initialization hints for GLFW
		// (see https://www.glfw.org/docs/latest/intro_guide.html#init_hints_values)

		// glfw error callback should eventually also log the error
		glfwSetErrorCallback(&glfw_error_callback);

		if (glfwInit() != GLFW_TRUE)
			throw std::runtime_error("Failed to initialize GLFW");
	}

	void Engine::closeLibraries() {
		glfwTerminate(); // destroys all remaining windows etc		
	}
}