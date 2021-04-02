#pragma once

#include "../preprocessor.h"

#include <thread>
#include <array>
#include <span>

namespace scimitar::os {
	class VSync {
	public:
		VSync();
		~VSync();

	private:
#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
		void openAdapter() noexcept;
		void closeAdapter() noexcept;

		void vsyncThread() noexcept;

		

		enum class eState {
			adapter_open,
			adapter_closed,
			fallback
		} m_State;

		void*    m_GDI             = nullptr;
		uint32_t m_Adapter         = 0;
		uint32_t m_PresentSourceID = 0;
		bool     m_ShouldStop      = false;

		std::thread m_VSyncThread;

#endif
	};
}