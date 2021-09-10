#include "os.h"
#include "../core/engine.h"

#include <iostream>

namespace scimitar {
	void OS::update() {
		MSG msg = {};

		// NOTE this *might* block the system; probably should introduce timing restrictions
		while (PeekMessageW(
			&msg,
			nullptr,
			0, // wMsgFilterMin
			0, // wMsgFilterMax
			PM_REMOVE
		)) {
			if (msg.message == WM_QUIT)
				m_Engine->stop();

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}