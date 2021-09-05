#pragma once

#include "../dependencies.h"

namespace scimitar::os {
	class GPU_Queue {
	public:
		GPU_Queue(
			uint32_t                family_queue_idx,
			uint32_t                queue_idx,
			vk::QueueFlags          flags,
			vk::Queue&&             queue,
			vk::UniqueCommandPool&& pool
		);

		uint32_t get_family_queue_idx() const noexcept;
		uint32_t get_queue_idx() const noexcept;

		vk::QueueFlags  get_queueFlags()  const noexcept;
		vk::Queue       get_queue()       const noexcept;
		vk::CommandPool get_pool() const noexcept;

		// convenience functions, although multiple of these may be true at the same time
		bool is_graphics()       const noexcept;
		bool is_compute()        const noexcept;
		bool is_transfer()       const noexcept;
		bool is_sparse_binding() const noexcept;
		bool is_protected()      const noexcept;

	private:
		uint32_t              m_FamilyQueueIdx;
		uint32_t              m_QueueIdx;
		vk::QueueFlags        m_QueueFlags;
		vk::Queue             m_Queue;
		vk::UniqueCommandPool m_CommandPool;
	};
}