#include "render_surface.h"

namespace scimitar::os {
	RenderSurface::RenderSurface(vk::UniqueSurfaceKHR&& surface):
		m_VkSurface(std::move(surface))
	{
		
	}

	RenderSurface::~RenderSurface() {

	}

	vk::SurfaceKHR RenderSurface::get_vk_surface() const noexcept {
		return m_VkSurface.get();
	}

	vk::SwapchainKHR RenderSurface::get_vk_swapchain() const noexcept {
		return m_VkSwapchain.get();
	}
}