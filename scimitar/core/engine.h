#pragma once

#include <atomic>
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>

#include "system.h"
#include "../app/application.h"
#include "../util/typemap.h"

/*
*	The Engine coordinates subsystem configuration and task execution
*/
namespace scimitar::core {
	class Engine {
	private:
		using ApplicationPtr = std::unique_ptr<app::Application>;
		using SystemPtr      = std::unique_ptr<System>;

		Engine();

	public:
		~Engine();

		Engine             (const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
		Engine             (Engine&&)      = delete;
		Engine& operator = (Engine&&)      = delete;

		static Engine& instance();

		void start();
		void stop();

		template <c_System T, typename... tArgs>
		T* add(tArgs... args);

		template <c_System T>
		void remove(T* system = nullptr); // type is relevant, actual pointer value not so much

		template <c_System T>
		const T* get() const; // fetch a subsystem of a given type

		template <c_System T>
		T* get();

		template <app::cApplication T, typename... tArgs>
		void set_application(tArgs... args);

	private:
		void start_libraries();
		void stop_libraries();

		void start_systems(); // loads settings, figures out in which order to resolve dependencies
		void stop_systems();  // saves settings, cleans up subsystems in reverse init order

		std::atomic_bool m_Running = false;

		std::mutex                m_SystemMutex;
		std::condition_variable   m_SystemCondition;

		std::vector<SystemPtr>    m_Systems;
		std::vector<std::jthread> m_DedicatedThreads;
		bool                      m_DedicatedSync = false;
		util::TypeMap             m_SystemMap;
		ApplicationPtr            m_Application;

		std::vector<std::string> m_InitOrder; // so that cleanup can be done in reverse
	};
}

#include "engine.inl"