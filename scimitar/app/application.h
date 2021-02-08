#pragma once

#include "../core/system.h"

namespace scimitar::app {
	class Application:
		public core::System
	{
	public:
		explicit Application(const std::string& name);

		virtual void init()     = 0;
		virtual void update()   = 0;
		virtual void shutdown() = 0;
	};

	template <typename T>
	concept cApplication = std::derived_from<T, Application>;
}