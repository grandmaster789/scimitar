#pragma once

#include <mutex>
#include <vector>
#include <functional>
#include "../../util/spin_mutex.h"

namespace scimitar::core::detail {
	template <typename T>
	class MediatorQueue {
	private:
		MediatorQueue() = default;

	public:
		static MediatorQueue& instance();

		template <typename H>
		void add(H* handler);

		template <typename H>
		void remove(H* handler);

		void remove_all();

		void broadcast(const T& message);

	private:
		using Mutex     = util::SpinMutex;
		using LockGuard = std::lock_guard<Mutex>;
		using Handler   = std::function<void(const T&)>;

		// using basic_string as a variant of <vector>, 
		// because it has small object optimization
		Mutex m_Mutex;
		std::vector<Handler> m_Handlers;
		std::vector<void*>   m_SourcePtrs;
	};
} // namespace scimitar::core::detail

#include "mediator_queue.inl"