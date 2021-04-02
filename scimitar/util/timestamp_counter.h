#pragma once

#include <atomic>
#include <cstdint>
#include <array>

// windows' QueryPerformanceCounter counts at 10Mhz, which is too slow for 
// many performance measurement applications.
// Instead we will use the TSC

namespace scimitar::util {
	class TimestampCounter {
	public:
		constexpr TimestampCounter() noexcept;
		constexpr TimestampCounter(
			uint64_t count,
			uint32_t cpuid
		) noexcept;

	private:
		uint64_t m_Count;

		inline static std::atomic<uint64_t> m_Period    = 0;
		inline static std::atomic<bool>     m_HaveCPUid = false;
		inline static std::atomic<size_t>   m_NumCPUids = 0;
		inline static std::array<uint32_t, 
	};
}