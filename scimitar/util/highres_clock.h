#pragma once

#include <cstdint>
#include <chrono>
#include <atomic>
#include <thread>

namespace scimitar::util {
	class TimeStampCounter;

	class HighresClock {
	public:
		using Representation = int64_t;
		using Period         = std::nano;
		using Duration       = std::chrono::duration<Representation, Period>;
		using TimePoint      = std::chrono::time_point<HighresClock>;

		static TimePoint now() noexcept;
		static TimePoint now()

	private:


	};
}