#pragma once

#include "../os/window.h"

#include <vulkan/vulkan.hpp>
#include <vector>
#include <memory>

/*
*	The Engine coordinates subsystem configuration and task execution
*/
namespace scimitar::core {
	class Engine {
	private:
		Engine();

	public:
		~Engine();

		Engine             (const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
		Engine             (Engine&&)      = delete;
		Engine& operator = (Engine&&)      = delete;

		static Engine& instance();

		os::Window* createWindow(
			const char* title  = "Scimitar", // consider using std string here?
			int         width  = 1280,
			int         height = 720
		);

		void start();
		void stop();

	private:
		void initLibraries();
		void closeLibraries();

		using WindowPtr = std::unique_ptr<os::Window>;

		std::vector<WindowPtr> m_Windows;
	};
}