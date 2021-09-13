#pragma once

#include "mediator_queue.h"
#include "../logger.h"
#include "../../util/algorithm.h"

namespace scimitar::core::detail{
	template <typename T>
	MediatorQueue<T>& MediatorQueue<T>::instance() {
		static MediatorQueue mq; // This ends up creating a specific queue for each template instance
		return mq;
	}

	template <typename T>
	template <typename H>
	void MediatorQueue<T>::add(H* handler) {
		LockGuard guard(m_Mutex);

		m_Handlers  .push_back([handler](const T& message) { (*handler)(message); });
		m_SourcePtrs.push_back(handler);
	}

	template <typename T>
	template <typename H>
	void MediatorQueue<T>::remove(H* handler) {
		LockGuard guard(m_Mutex);

		auto it = util::find(m_SourcePtrs, handler);

		if (it != std::end(m_SourcePtrs)) {
			 size_t idx = std::distance(std::cbegin(m_SourcePtrs), it);

			m_SourcePtrs.erase(it);
			m_Handlers.erase(std::begin(m_Handlers) + idx);
		}
		else
			gLogWarning << "Tried to remove an unregistered handler";
	}

	template <typename T>
	void MediatorQueue<T>::remove_all() {
		LockGuard guard(m_Mutex);

		m_SourcePtrs.clear();
		m_Handlers.clear();
	}

	template <typename T>
	void MediatorQueue<T>::broadcast(const T& message) {
		decltype(m_Handlers) local_copy;

		{
			LockGuard guard(m_Mutex);
			local_copy = m_Handlers;
		}

		// by using a local copy of the list we allow
		// handlers to mutate the set (typically by removing 
		// themselves from the set)
		for (const auto& handler : local_copy)
			handler(message);
	}
}