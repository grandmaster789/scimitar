#pragma once

#include <thread>
#include <string_view>
#include <functional>
#include <cstdint>
#include <vector>

#include "../preprocessor.h"

namespace scimitar::util {
#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS
	constexpr size_t c_MaxNumCPUs = 64;
	using            ThreadID = uint32_t;
#else
	constexpr size_t    c_MaxNumCPUs                      = CPU_SETSIZE; // https://man7.org/linux/man-pages/man3/CPU_SET.3.html
	using               ThreadID                          = uint64_t;
	inline thread_local ThreadID c_CurrentThreadID_helper = 0;
#endif

	void              set_thread_name          (std::string_view name);
	bool              is_main_thread           ();
	void              run_from_main_thread     (std::function<void()> fn);
	ThreadID          get_current_threadID     () noexcept;
	size_t            get_current_cpuID        () noexcept;
	std::vector<bool> get_process_affinity_mask() noexcept;                     // yields a bitmask on which CPUs the process is allowed to run on
	std::vector<bool> set_thread_affinity_mask (const std::vector<bool>& mask); // parameter must be a subset of get_process_affinity_mask and indicates which CPUs the process is allowed to run on (returns previous mask)
	std::vector<bool> set_thread_affinity      (size_t  cpuID);                 // set the current thread affinity to a single cpu (returns previous mask)
	size_t            advance_thread_affinity  (size_t& cpuID) noexcept;        // sets the parameter to the next available cpu, returns the cpu selected to run on
}
