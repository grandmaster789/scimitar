#pragma once

#include "../dependencies.h"
#include "../geometry/extent_2d.h"
#include "../geometry/aa_rectangle.h"

namespace scimitar::os {
	class RenderDevice;

	struct SwapchainImageInfo {
		vk::Image                      m_Image;
		vk::ImageView                  m_View;
		vk::Framebuffer                m_Frame;
		geometry::AxisAlignedRectangle m_RedrawArea;
		bool                           m_HasLayout = false;
	};

	enum class eRenderSurfaceState {
		initializing,   // no window available yet
		no_window,      // window was destroyed
		no_device,      // no render device was set
		no_surface,     // surface is no longer available, need to request a new one
		ready,          // available for rendering
		swapchain_lost, // most probably the window was resized
		surface_lost,   // 
		device_lost,    //
		window_lost
	};

	/*
	*	NOTE This is not immediately initialized upon construction;
	*	first construct, then set the render device, after which you can call init()
	*/
	class RenderSurface {
	public:
		RenderSurface(vk::UniqueSurfaceKHR&& surface);
		~RenderSurface();

		// no-copy, no-move
		RenderSurface             (const RenderSurface&) = delete;
		RenderSurface& operator = (const RenderSurface&) = delete;
		RenderSurface             (RenderSurface&&)      = delete;
		RenderSurface& operator = (RenderSurface&&)      = delete;

		void          setDevice(RenderDevice* device) noexcept;
		RenderDevice* getDevice() const noexcept;

		void setClosed() noexcept;
		bool isClosed() const noexcept;

		vk::SurfaceKHR   get_vk_surface()   const noexcept;
		vk::SwapchainKHR get_vk_swapchain() const noexcept;

	private:
		static constexpr uint32_t   m_DefaultNumSwapchainImages = 2;
		static constexpr vk::Format m_DefaultDepthImageFormat   = vk::Format::eD32Sfloat;
		static constexpr vk::Format m_DefaultColorImageFormat   = vk::Format::eR16G16B16A16Sfloat;

		RenderDevice* m_Device = nullptr;

		eRenderSurfaceState m_State = eRenderSurfaceState::no_device;

		vk::UniqueSurfaceKHR   m_VkSurface;
		vk::UniqueSwapchainKHR m_VkSwapchain;

		uint32_t                        m_NumSwapchainImages;
		vk::Extent2D                    m_SwapchainImageExtent;
		vk::SurfaceFormatKHR            m_SwapchainImageFormat;
		std::vector<SwapchainImageInfo> m_SwapchainInfoList;

		VmaAllocation m_DepthImageAllocation;
		vk::Image     m_DepthImage;
		vk::ImageView m_DepthView;

		std::array<VmaAllocation, 2>           m_ColorImageAllocations;
		std::array<vk::Image, 2>               m_ColorImages;
		std::array<vk::ImageView, 2>           m_ColorViews;
		std::array<vk::DescriptorImageInfo, 2> m_ColorDescriptorInfos;

		vk::RenderPass    m_RenderPass;
		vk::CommandBuffer m_CommandBuffer;

		vk::Semaphore m_ImageAvailableSemaphore;
		vk::Semaphore m_RenderFinishedSemaphore;
		vk::Fence     m_RenderFinishedFence;
	};
}