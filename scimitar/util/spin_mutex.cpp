#include "spin_mutex.h"
#include <thread>

namespace scimitar::util {
	void SpinMutex::lock() noexcept {
		do {
			// spin until this is no longer in the 'locked' state
			// (no particular constraints here)
			while (m_Locked.load(std::memory_order_relaxed)) {
				std::this_thread::yield();
			}
		} 
		// try and set the locked state using an atomic acquire operation
		// (if it fails, continue to spin and try again later)
		while (m_Locked.exchange(true, std::memory_order_acquire)); 
	}

	void SpinMutex::unlock() noexcept {
		// atomic release operation
		m_Locked.store(false, std::memory_order_release);
	}
}