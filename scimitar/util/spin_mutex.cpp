#include "spin_mutex.h"
#include <thread>

namespace scimitar::util {
	void SpinMutex::lock() noexcept {

	}

	bool SpinMutex::try_lock() noexcept {
		return false;
	}

	void SpinMutex::unlock() noexcept {
		
	}
}