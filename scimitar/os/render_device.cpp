#include "render_device.h"
#include "os.h"
#include "../util/debugger.h"
#include "render_surface.h"

namespace {
	bool has_required_features(
		const vk::PhysicalDevice&         device, 
		const vk::PhysicalDeviceFeatures& reqs
	) {
		auto features = device.getFeatures();

		bool result = true;

		// alphabetically ordered, full set at the moment of writing is tested
		// (NOTE at the time of writing, we only require robust buffer access for debug builds)
		if (reqs.alphaToOne                              == VK_TRUE) result &= (features.alphaToOne                              == VK_TRUE);
		if (reqs.depthBiasClamp                          == VK_TRUE) result &= (features.depthBiasClamp                          == VK_TRUE);
		if (reqs.depthBounds                             == VK_TRUE) result &= (features.depthBounds                             == VK_TRUE);
		if (reqs.depthClamp                              == VK_TRUE) result &= (features.depthClamp                              == VK_TRUE);
		if (reqs.drawIndirectFirstInstance               == VK_TRUE) result &= (features.drawIndirectFirstInstance               == VK_TRUE);
		if (reqs.dualSrcBlend                            == VK_TRUE) result &= (features.dualSrcBlend                            == VK_TRUE);
		if (reqs.fillModeNonSolid                        == VK_TRUE) result &= (features.fillModeNonSolid                        == VK_TRUE);
		if (reqs.fragmentStoresAndAtomics                == VK_TRUE) result &= (features.fragmentStoresAndAtomics                == VK_TRUE);
		if (reqs.fullDrawIndexUint32                     == VK_TRUE) result &= (features.fullDrawIndexUint32                     == VK_TRUE);
		if (reqs.geometryShader                          == VK_TRUE) result &= (features.geometryShader                          == VK_TRUE);
		if (reqs.imageCubeArray                          == VK_TRUE) result &= (features.imageCubeArray                          == VK_TRUE);
		if (reqs.independentBlend                        == VK_TRUE) result &= (features.independentBlend                        == VK_TRUE);
		if (reqs.inheritedQueries                        == VK_TRUE) result &= (features.inheritedQueries                        == VK_TRUE);
		if (reqs.largePoints                             == VK_TRUE) result &= (features.largePoints                             == VK_TRUE);
		if (reqs.logicOp                                 == VK_TRUE) result &= (features.logicOp                                 == VK_TRUE);
		if (reqs.multiDrawIndirect                       == VK_TRUE) result &= (features.multiDrawIndirect                       == VK_TRUE);
		if (reqs.multiViewport                           == VK_TRUE) result &= (features.multiViewport                           == VK_TRUE);
		if (reqs.occlusionQueryPrecise                   == VK_TRUE) result &= (features.occlusionQueryPrecise                   == VK_TRUE);
		if (reqs.pipelineStatisticsQuery                 == VK_TRUE) result &= (features.pipelineStatisticsQuery                 == VK_TRUE);
		if (reqs.robustBufferAccess                      == VK_TRUE) result &= (features.robustBufferAccess                      == VK_TRUE);
		if (reqs.samplerAnisotropy                       == VK_TRUE) result &= (features.samplerAnisotropy                       == VK_TRUE);
		if (reqs.sampleRateShading                       == VK_TRUE) result &= (features.sampleRateShading                       == VK_TRUE);
		if (reqs.shaderClipDistance                      == VK_TRUE) result &= (features.shaderClipDistance                      == VK_TRUE);
		if (reqs.shaderCullDistance                      == VK_TRUE) result &= (features.shaderCullDistance                      == VK_TRUE);
		if (reqs.shaderFloat64                           == VK_TRUE) result &= (features.shaderFloat64                           == VK_TRUE);
		if (reqs.shaderImageGatherExtended               == VK_TRUE) result &= (features.shaderImageGatherExtended               == VK_TRUE);
		if (reqs.shaderInt16                             == VK_TRUE) result &= (features.shaderInt16                             == VK_TRUE);
		if (reqs.shaderInt64                             == VK_TRUE) result &= (features.shaderInt64                             == VK_TRUE);
		if (reqs.shaderResourceMinLod                    == VK_TRUE) result &= (features.shaderResourceMinLod                    == VK_TRUE);
		if (reqs.shaderResourceResidency                 == VK_TRUE) result &= (features.shaderResourceResidency                 == VK_TRUE);
		if (reqs.shaderSampledImageArrayDynamicIndexing  == VK_TRUE) result &= (features.shaderSampledImageArrayDynamicIndexing  == VK_TRUE);
		if (reqs.shaderStorageBufferArrayDynamicIndexing == VK_TRUE) result &= (features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE);
		if (reqs.shaderStorageImageArrayDynamicIndexing  == VK_TRUE) result &= (features.shaderStorageImageArrayDynamicIndexing  == VK_TRUE);
		if (reqs.shaderStorageImageExtendedFormats       == VK_TRUE) result &= (features.shaderStorageImageExtendedFormats       == VK_TRUE);
		if (reqs.shaderStorageImageMultisample           == VK_TRUE) result &= (features.shaderStorageImageMultisample           == VK_TRUE);
		if (reqs.shaderStorageImageReadWithoutFormat     == VK_TRUE) result &= (features.shaderStorageImageReadWithoutFormat     == VK_TRUE);
		if (reqs.shaderStorageImageWriteWithoutFormat    == VK_TRUE) result &= (features.shaderStorageImageWriteWithoutFormat    == VK_TRUE);
		if (reqs.shaderTessellationAndGeometryPointSize  == VK_TRUE) result &= (features.shaderTessellationAndGeometryPointSize  == VK_TRUE);
		if (reqs.shaderUniformBufferArrayDynamicIndexing == VK_TRUE) result &= (features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
		if (reqs.sparseBinding                           == VK_TRUE) result &= (features.sparseBinding                           == VK_TRUE);
		if (reqs.sparseResidency16Samples                == VK_TRUE) result &= (features.sparseResidency16Samples                == VK_TRUE);
		if (reqs.sparseResidency2Samples                 == VK_TRUE) result &= (features.sparseResidency2Samples                 == VK_TRUE);
		if (reqs.sparseResidency4Samples                 == VK_TRUE) result &= (features.sparseResidency4Samples                 == VK_TRUE);
		if (reqs.sparseResidency8Samples                 == VK_TRUE) result &= (features.sparseResidency8Samples                 == VK_TRUE);
		if (reqs.sparseResidencyAliased                  == VK_TRUE) result &= (features.sparseResidencyAliased                  == VK_TRUE);
		if (reqs.sparseResidencyBuffer                   == VK_TRUE) result &= (features.sparseResidencyBuffer                   == VK_TRUE);
		if (reqs.sparseResidencyImage2D                  == VK_TRUE) result &= (features.sparseResidencyImage2D                  == VK_TRUE);
		if (reqs.sparseResidencyImage3D                  == VK_TRUE) result &= (features.sparseResidencyImage3D                  == VK_TRUE);
		if (reqs.tessellationShader                      == VK_TRUE) result &= (features.tessellationShader                      == VK_TRUE);
		if (reqs.textureCompressionASTC_LDR              == VK_TRUE) result &= (features.textureCompressionASTC_LDR              == VK_TRUE);
		if (reqs.textureCompressionBC                    == VK_TRUE) result &= (features.textureCompressionBC                    == VK_TRUE);
		if (reqs.textureCompressionETC2                  == VK_TRUE) result &= (features.textureCompressionETC2                  == VK_TRUE);
		if (reqs.variableMultisampleRate                 == VK_TRUE) result &= (features.variableMultisampleRate                 == VK_TRUE);
		if (reqs.vertexPipelineStoresAndAtomics          == VK_TRUE) result &= (features.vertexPipelineStoresAndAtomics          == VK_TRUE);

		return result;
	}

	bool has_required_limits(
		const vk::PhysicalDevice&       device,
		const vk::PhysicalDeviceLimits& reqs
	) {
		auto limits = device.getProperties().limits;

		bool result = true;

		// not all limits make sense to test, we only do a partial comparison here (and there may be some nonsensical stuff in here)
		// (NOTE in fact we currently have no specific requirements for the project yet)
		// TODO: pick through this and make all comparisons in the correct direction
		result &= (limits.maxBoundDescriptorSets                          >= reqs.maxBoundDescriptorSets);
		result &= (limits.maxClipDistances                                >= reqs.maxClipDistances);
		result &= (limits.maxColorAttachments                             >= reqs.maxColorAttachments);
		result &= (limits.maxCombinedClipAndCullDistances                 >= reqs.maxCombinedClipAndCullDistances);
		result &= (limits.maxComputeSharedMemorySize                      >= reqs.maxComputeSharedMemorySize);
		result &= (limits.maxComputeWorkGroupCount                        >= reqs.maxComputeWorkGroupCount);
		result &= (limits.maxComputeWorkGroupInvocations                  >= reqs.maxComputeWorkGroupInvocations);
		result &= (limits.maxComputeWorkGroupSize                         >= reqs.maxComputeWorkGroupSize);
		result &= (limits.maxCullDistances                                >= reqs.maxCullDistances);
		result &= (limits.maxDescriptorSetInputAttachments                >= reqs.maxDescriptorSetInputAttachments);
		result &= (limits.maxDescriptorSetSampledImages                   >= reqs.maxDescriptorSetSampledImages);
		result &= (limits.maxDescriptorSetSamplers                        >= reqs.maxDescriptorSetSamplers);
		result &= (limits.maxDescriptorSetStorageBuffers                  >= reqs.maxDescriptorSetStorageBuffers);
		result &= (limits.maxDescriptorSetStorageBuffersDynamic           >= reqs.maxDescriptorSetStorageBuffersDynamic);
		result &= (limits.maxDescriptorSetStorageImages                   >= reqs.maxDescriptorSetStorageImages);
		result &= (limits.maxDescriptorSetUniformBuffers                  >= reqs.maxDescriptorSetUniformBuffers);
		result &= (limits.maxDescriptorSetUniformBuffersDynamic           >= reqs.maxDescriptorSetUniformBuffersDynamic);
		result &= (limits.maxDrawIndexedIndexValue                        >= reqs.maxDrawIndexedIndexValue);
		result &= (limits.maxDrawIndirectCount                            >= reqs.maxDrawIndirectCount);
		result &= (limits.maxFragmentCombinedOutputResources              >= reqs.maxFragmentCombinedOutputResources);
		result &= (limits.maxFragmentDualSrcAttachments                   >= reqs.maxFragmentDualSrcAttachments);
		result &= (limits.maxFragmentInputComponents                      >= reqs.maxFragmentInputComponents);
		result &= (limits.maxFragmentOutputAttachments                    >= reqs.maxFragmentOutputAttachments);
		result &= (limits.maxFramebufferHeight                            >= reqs.maxFramebufferHeight);
		result &= (limits.maxFramebufferLayers                            >= reqs.maxFramebufferLayers);
		result &= (limits.maxFramebufferWidth                             >= reqs.maxFramebufferWidth);
		result &= (limits.maxGeometryInputComponents                      >= reqs.maxGeometryInputComponents);
		result &= (limits.maxGeometryOutputComponents                     >= reqs.maxGeometryOutputComponents);
		result &= (limits.maxGeometryOutputVertices                       >= reqs.maxGeometryOutputVertices);
		result &= (limits.maxGeometryShaderInvocations                    >= reqs.maxGeometryShaderInvocations);
		result &= (limits.maxGeometryTotalOutputComponents                >= reqs.maxGeometryTotalOutputComponents);
		result &= (limits.maxImageArrayLayers                             >= reqs.maxImageArrayLayers);
		result &= (limits.maxImageDimension1D                             >= reqs.maxImageDimension1D);
		result &= (limits.maxImageDimension2D                             >= reqs.maxImageDimension2D);
		result &= (limits.maxImageDimension3D                             >= reqs.maxImageDimension3D);
		result &= (limits.maxImageDimensionCube                           >= reqs.maxImageDimensionCube);
		result &= (limits.maxInterpolationOffset                          >= reqs.maxInterpolationOffset);
		result &= (limits.maxMemoryAllocationCount                        >= reqs.maxMemoryAllocationCount);
		result &= (limits.maxPerStageDescriptorInputAttachments           >= reqs.maxPerStageDescriptorInputAttachments);
		result &= (limits.maxPerStageDescriptorSampledImages              >= reqs.maxPerStageDescriptorSampledImages);
		result &= (limits.maxPerStageDescriptorSamplers                   >= reqs.maxPerStageDescriptorSamplers);
		result &= (limits.maxPerStageDescriptorStorageBuffers             >= reqs.maxPerStageDescriptorStorageBuffers);
		result &= (limits.maxPerStageDescriptorStorageImages              >= reqs.maxPerStageDescriptorStorageImages);
		result &= (limits.maxPerStageDescriptorUniformBuffers             >= reqs.maxPerStageDescriptorUniformBuffers);
		result &= (limits.maxPerStageResources                            >= reqs.maxPerStageResources);
		result &= (limits.maxPushConstantsSize                            >= reqs.maxPushConstantsSize);
		result &= (limits.maxSampleMaskWords                              >= reqs.maxSampleMaskWords);
		result &= (limits.maxSamplerAllocationCount                       >= reqs.maxSamplerAllocationCount);
		result &= (limits.maxSamplerAnisotropy                            >= reqs.maxSamplerAnisotropy);
		result &= (limits.maxSamplerLodBias                               >= reqs.maxSamplerLodBias);
		result &= (limits.maxStorageBufferRange                           >= reqs.maxStorageBufferRange);
		result &= (limits.maxTessellationControlPerPatchOutputComponents  >= reqs.maxTessellationControlPerPatchOutputComponents);
		result &= (limits.maxTessellationControlPerVertexInputComponents  >= reqs.maxTessellationControlPerVertexInputComponents);
		result &= (limits.maxTessellationControlPerVertexOutputComponents >= reqs.maxTessellationControlPerVertexOutputComponents);
		result &= (limits.maxTessellationControlTotalOutputComponents     >= reqs.maxTessellationControlTotalOutputComponents);
		result &= (limits.maxTessellationEvaluationInputComponents        >= reqs.maxTessellationEvaluationInputComponents);
		result &= (limits.maxTessellationEvaluationOutputComponents       >= reqs.maxTessellationEvaluationOutputComponents);
		result &= (limits.maxTessellationGenerationLevel                  >= reqs.maxTessellationGenerationLevel);
		result &= (limits.maxTessellationPatchSize                        >= reqs.maxTessellationGenerationLevel);
		result &= (limits.maxTexelBufferElements                          >= reqs.maxTexelBufferElements);
		result &= (limits.maxTexelGatherOffset                            >= reqs.maxTexelGatherOffset);
		result &= (limits.maxTexelOffset                                  >= reqs.maxTexelOffset);
		result &= (limits.maxVertexInputAttributeOffset                   >= reqs.maxVertexInputAttributeOffset);
		result &= (limits.maxVertexInputAttributes                        >= reqs.maxVertexInputAttributeOffset);
		result &= (limits.maxVertexInputBindings                          >= reqs.maxVertexInputBindings);
		result &= (limits.maxVertexInputBindingStride                     >= reqs.maxVertexInputBindingStride);
		result &= (limits.maxVertexOutputComponents                       >= reqs.maxVertexOutputComponents);

		return result;
	}
}

namespace scimitar::os {
	RenderDevice::RenderDevice(OS* os, vk::PhysicalDevice physical):
		m_PhysicalDevice(physical)
	{
		auto props2        = physical.getProperties2();
		m_DeviceProperties = props2.properties;
		m_DeviceName       = std::string(&m_DeviceProperties.deviceName[0]); // convert from vk::ArrayWrapper<char, 256>
		
		// initialize the logical device
		std::array<float, 1> default_queue_priority = { 1.0f };
		uint32_t             queue_family_index     = 0;

		std::vector<vk::DeviceQueueCreateInfo> dqci;
		
		for ([[maybe_unused]] const auto& properties: m_PhysicalDevice.getQueueFamilyProperties()) {
			uint32_t num_queues = 1;

			vk::DeviceQueueCreateInfo info(
				vk::DeviceQueueCreateFlags(),
				queue_family_index++,
				num_queues,
				default_queue_priority.data()
			);

			dqci.push_back(info);
		}

		m_RequiredFeatures = os->get_vk_required_physical_features();
		m_RequiredLimits   = os->get_vk_required_physical_limits();

		m_RequiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
		m_RequiredExtensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);

		vk::DeviceCreateInfo dci;
		
		dci.setQueueCreateInfoCount  (static_cast<uint32_t>(dqci.size()));
		dci.setQueueCreateInfos      (dqci);
		dci.setEnabledExtensionCount (static_cast<uint32_t>(m_RequiredExtensions.size()));
		dci.setPEnabledExtensionNames(m_RequiredExtensions);
		dci.setPEnabledFeatures      (&m_RequiredFeatures);
		
		m_Device = m_PhysicalDevice.createDeviceUnique(dci);

		// initialize the memory allocator
		VmaAllocatorCreateInfo vaci = {};
		vaci.physicalDevice = m_PhysicalDevice;
		vaci.device         = get_device();
		vaci.instance       = os->get_vk_instance();
		vmaCreateAllocator(&vaci, &m_Allocator);

		// figure out if lazy transient images are supported
		VmaAllocationCreateInfo lazy_alloc_info = {};
		lazy_alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
		uint32_t type_index = 0;

		if (vmaFindMemoryTypeIndex(m_Allocator, 0, &lazy_alloc_info, &type_index) != VkResult::VK_ERROR_FEATURE_NOT_PRESENT) {
			m_SupportsLazyTransientImages = true;
			m_TransientImageUsageFlags   &= vk::ImageUsageFlagBits::eTransientAttachment;
			m_LazyMemoryUsage             = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
		}

		// initialize the queues, create command pools for everything
		auto family_prop_list = m_PhysicalDevice.getQueueFamilyProperties();

		for (const auto& info : dqci) {
			uint32_t   queue_family_idx = info.queueFamilyIndex;
			const auto props            = family_prop_list[queue_family_idx];
			const auto flags            = props.queueFlags;

			for (uint32_t queue_idx = 0; queue_idx != info.queueCount; ++queue_idx) {
				auto vk_queue = get_device().getQueue(queue_family_idx, queue_idx);

				vk::CommandPoolCreateInfo cpci(
					vk::CommandPoolCreateFlagBits::eTransient |
					vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
					queue_family_idx
				);

				auto commandpool = get_device().createCommandPoolUnique(cpci);

				GPU_Queue queue(
					queue_family_idx,
					queue_idx,
					flags,
					std::move(vk_queue),
					std::move(commandpool)
				);

				m_Queues.push_back(std::move(queue));
			}
		}

	}

	RenderDevice::~RenderDevice() {
		try {

		}
		catch (std::exception& e) {
			debugger_abort(e.what());
		}
	}

	vk::Device RenderDevice::get_device() const {
		return m_Device.get();
	}

	const GPU_Queue& RenderDevice::get_graphics_queue() const {
		for (const auto& queue : m_Queues)
			if (queue.is_graphics())
				return queue;

		throw std::runtime_error("No graphics queue availaible at all");
	}

	const GPU_Queue& RenderDevice::get_graphics_queue(const RenderSurface& surface) const {
		const GPU_Queue* fallback = nullptr;
		auto vk_surface = surface.get_vk_surface();

		// try to find something that can present to the given surface
		for (const auto& queue : m_Queues) {
			if (queue.is_graphics()) {
				if (m_PhysicalDevice.getSurfaceSupportKHR(
					queue.get_family_queue_idx(),
					vk_surface
				))
					return queue;

				// keep the first graphics queue as a fallback jic
				if (!fallback)
					fallback = &queue;
			}
		}

		if (fallback == nullptr)
			throw std::runtime_error("No graphics queue available at all");

		return *fallback;
	}

	const GPU_Queue& RenderDevice::get_present_queue(const RenderSurface& surface) const {
		auto vk_surface = surface.get_vk_surface();

		for (const auto& queue : m_Queues) {
			if (queue.is_graphics()) {
				if (m_PhysicalDevice.getSurfaceSupportKHR(
					queue.get_family_queue_idx(),
					vk_surface
				))
					return queue;
			}
		}

		throw std::runtime_error("None of the queues may present");
	}

	int RenderDevice::score(const RenderSurface& surface) const noexcept {
		auto vksurf = surface.get_vk_surface();

		int result = 0;

		// if the physical device doesn't meet the requirements yield -1
		if (!has_required_features(m_PhysicalDevice, m_RequiredFeatures))
			return -1;

		if (!has_required_limits(m_PhysicalDevice, m_RequiredLimits))
			return -1;

		bool has_graphics_queue          = false;
		bool has_compute_queue           = false;
		bool can_present                 = false;
		bool shared_graphics_and_present = false; // if the present and graphics queue aren't shared a transfer will need to occur before presentation, so sharing it is generally better

		for (const auto& queue : m_Queues) {
			bool graphicsable = queue.is_graphics();
			bool computable   = queue.is_compute();
			bool presentable  = (m_PhysicalDevice.getSurfaceSupportKHR(
				queue.get_family_queue_idx(), 
				surface.get_vk_surface()
			) == VK_TRUE);

			has_graphics_queue |= graphicsable;
			has_compute_queue  |= computable;
			can_present        |= presentable;

			if (graphicsable && presentable)
				shared_graphics_and_present = true;
		}

		// if the device cannot do graphics yield -1
		if (!has_graphics_queue)
			return -1;

		// if the device cannot present yield -1
		if (!can_present)
			return -1;

		// compute is kinda cool
		if (has_compute_queue)
			result += 1;

		// sharing graphics and present is really cool
		if (shared_graphics_and_present)
			result += 5;

		// try to find a suitable surface format
		get_surface_format(surface, &result);

		// try to find a suitable present mode
		get_present_mode(surface, &result);

		// finally, prefer discrete GPUs to integrated GPUs to Virtual GPUs to CPUs to anything else
		switch (m_PhysicalDevice.getProperties().deviceType) {
		case vk::PhysicalDeviceType::eCpu:           result += 1; break;
		case vk::PhysicalDeviceType::eOther:         result += 1; break;
		case vk::PhysicalDeviceType::eVirtualGpu:    result += 2; break;
		case vk::PhysicalDeviceType::eIntegratedGpu: result += 3; break;
		case vk::PhysicalDeviceType::eDiscreteGpu:   result += 4; break;
		}

		return result;
	}


	vk::SurfaceFormatKHR RenderDevice::get_surface_format(
		const RenderSurface& surface, 
		int* score 
	) const noexcept {
		vk::SurfaceFormatKHR result;

		auto available = m_PhysicalDevice.getSurfaceFormatsKHR(surface.get_vk_surface());

		// internal scoring - prefer esRGB to sRGB to RGB
		// don't use the A2B10G10R10UnormPack32 (which is a wire format)
		// prefer floating point formats to sRGB formats to RGB
		int best = 0;

		for (auto fmt : available) {
			int x = 0;

			switch (fmt.colorSpace) {
			case vk::ColorSpaceKHR::eSrgbNonlinear:            x += 1;  break;
			case vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT: x += 10; break;
			default:
				break;
			}

			switch (fmt.format) {
			case vk::Format::eR16G16B16A16Sfloat:     x += 12;  break; // 64bpp HDRA
			case vk::Format::eR16G16B16Sfloat:        x += 11;  break; // 48bpp HDR

			case vk::Format::eR8G8B8A8Srgb:           x += 4;   break; // 32bpp sRGBA
			case vk::Format::eB8G8R8A8Srgb:           x += 4;   break; // 32bpp sBGRA
			case vk::Format::eR8G8B8Srgb:             x += 3;   break; // 24bpp sRGB
			case vk::Format::eB8G8R8Srgb:             x += 3;   break; // 24bpp sBGR
			case vk::Format::eR8G8B8A8Unorm:          x += 2;   break; // 32bpp RGBA
			case vk::Format::eB8G8R8A8Unorm:          x += 2;   break; // 32bpp BGRA
			case vk::Format::eR8G8B8Unorm:            x += 1;   break; // 24bpp RGB
			case vk::Format::eB8G8R8Unorm:            x += 1;   break; // 24bpp BGR

			case vk::Format::eA2B10G10R10UnormPack32: x -= 9999; break; // <- do not use this

			// the other formats aren't completely garbage but will cause additional transformations
			default:
				break;
			}

			if (x > best) {
				best = x;
				result = fmt;
			}
		}

		if (score)
			*score += best;

		return result;
	}

	vk::PresentModeKHR RenderDevice::get_present_mode(
		const RenderSurface& surface,
		int* score
	) const noexcept {
		// prefer Fifo to FifoRelaxed to immediate/mailbox
		// TODO take a look at SharedDemandRefresh and SharedContinuousRefresh

		vk::PresentModeKHR result = vk::PresentModeKHR::eMailbox; // should be available on all graphics devices

		int best = 0;

		auto available = m_PhysicalDevice.getSurfacePresentModesKHR(surface.get_vk_surface());

		for (auto mode : available) {
			int x = 0;

			switch (mode) {
			case vk::PresentModeKHR::eFifo:        x += 3; break;
			case vk::PresentModeKHR::eFifoRelaxed: x += 2; break;
			case vk::PresentModeKHR::eImmediate:   x += 1; break;
			case vk::PresentModeKHR::eMailbox:     x += 1; break;

			case vk::PresentModeKHR::eSharedContinuousRefresh: // ?
			case vk::PresentModeKHR::eSharedDemandRefresh:     // ?
			default:
				continue;
			}

			if (x > best) {
				best = x;
				result = mode;
			}
		}

		if (score)
			*score += best;

		return result;
	}
}