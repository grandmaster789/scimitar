#pragma once

#include <atomic>

namespace scimitar::util {
	// https://en.cppreference.com/w/cpp/named_req/BasicLockable
	class SpinMutex {
	public:
		SpinMutex() noexcept = default;

		SpinMutex             (const SpinMutex&) = delete;
		SpinMutex& operator = (const SpinMutex&) = delete;

		void lock()   noexcept;
		void unlock() noexcept;

	private:
		std::atomic_bool m_Locked = false;
	};
}