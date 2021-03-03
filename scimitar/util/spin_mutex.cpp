#include "spin_mutex.h"
#include <thread>
#include <cassert>

namespace scimitar::util {
	void SpinMutex::lock() noexcept {
		assert(m_Semaphore.load() <= 2); // please only use this in low contention situations (max 1 waiter)

		uint32_t expected = 0;

		// if we manage to set 1 there are no waiters
		if (!m_Semaphore.compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
			[[unlikely]]
			lock_contention(expected);
		}

		assert(m_Semaphore.load() <= 2); // please only use this in low contention situations (max 1 waiter)
	}

	bool SpinMutex::try_lock() noexcept {
		assert(m_Semaphore.load() <= 2); // please only use this in low contention situations (max 1 waiter)

		uint32_t expected = 0;
		if (!m_Semaphore.compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
			[[unlikely]]
			return false;
		}

		assert(m_Semaphore.load() <= 2); // please only use this in low contention situations (max 1 waiter)
		return false;
	}

	void SpinMutex::unlock() noexcept {
		assert(m_Semaphore.load() <= 2); // please only use this in low contention situations (max 1 waiter)

		if (m_Semaphore.fetch_sub(1, std::memory_order_relaxed) != 1) {
		}
		else
			atomic_thread_fence(std::memory_order_release);
	}
}