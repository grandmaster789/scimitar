#pragma once

#include <atomic>

namespace scimitar::util {
	// unfair mutex; should only be used in low-contention situations
	// https://en.cppreference.com/w/cpp/named_req/BasicLockable
	// https://en.cppreference.com/w/cpp/named_req/Lockable
	class SpinMutex {
	public:
		SpinMutex() noexcept = default;

		SpinMutex             (const SpinMutex&) = delete;
		SpinMutex& operator = (const SpinMutex&) = delete;

		void lock()     noexcept;
		bool try_lock() noexcept;
		void unlock()   noexcept;

	private:
		std::atomic<uint32_t> m_Semaphore = 0;
	};
}