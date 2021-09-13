#include "os.h"
#include "../core/engine.h"

namespace scimitar {
	void OS::update() {
		if (!m_Windows.empty()) {
			MSG msg = {};

			// TODO handling OS events could be capped to try and get more consistent FPS

			while (PeekMessage(
				&msg,
				nullptr,  // hWnd
				0,        // msg filter min
				0,        // msg filter max
				PM_REMOVE // remove message from queue, if any
			)) {
				if (msg.message == WM_QUIT)
					m_Engine->stop();

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (m_Windows.empty())
				return;
		}
	}
}