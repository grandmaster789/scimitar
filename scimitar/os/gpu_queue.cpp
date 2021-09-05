#include "gpu_queue.h"

namespace scimitar::os {
	GPU_Queue::GPU_Queue(
		uint32_t                family_queue_idx,
		uint32_t                queue_idx,
		vk::QueueFlags          flags,
		vk::Queue&& queue,
		vk::UniqueCommandPool&& pool
	):
		m_FamilyQueueIdx(family_queue_idx),
		m_QueueIdx      (queue_idx),
		m_QueueFlags    (flags),
		m_Queue         (std::move(queue)),
		m_CommandPool   (std::move(pool))
	{
	}

	uint32_t GPU_Queue::get_family_queue_idx() const noexcept {
		return m_FamilyQueueIdx;
	}

	uint32_t GPU_Queue::get_queue_idx() const noexcept {
		return m_QueueIdx;
	}

	vk::QueueFlags GPU_Queue::get_queueFlags()  const noexcept {
		return m_QueueFlags;
	}

	vk::Queue GPU_Queue::get_queue() const noexcept {
		return m_Queue;
	}

	vk::CommandPool GPU_Queue::get_pool() const noexcept {
		return m_CommandPool.get();
	}

	bool GPU_Queue::is_graphics() const noexcept {
		return static_cast<bool>(m_QueueFlags & vk::QueueFlagBits::eGraphics);
	}

	bool GPU_Queue::is_compute() const noexcept {
		return static_cast<bool>(m_QueueFlags & vk::QueueFlagBits::eCompute);
	}

	bool GPU_Queue::is_transfer() const noexcept {
		return static_cast<bool>(m_QueueFlags & vk::QueueFlagBits::eTransfer);
	}

	bool GPU_Queue::is_sparse_binding() const noexcept {
		return static_cast<bool>(m_QueueFlags & vk::QueueFlagBits::eSparseBinding);
	}

	bool GPU_Queue::is_protected() const noexcept {
		return static_cast<bool>(m_QueueFlags & vk::QueueFlagBits::eProtected);
	}
}