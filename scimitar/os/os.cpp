#include "os.h"
#include "../util/algorithm.h"
#include "../util/debugger.h"
#include "../core/engine.h"
#include "../dependencies.h"
#include "render_device.h"

namespace {
	void list_instance_extensions() {
		auto exts = vk::enumerateInstanceExtensionProperties();

		debugger_log("Available Vulkan instance extensions ({}):", exts.size());
		for (const auto& prop : exts)
			debugger_log("\t{}", prop.extensionName);
	}

	void list_instance_layers() {
		auto layers = vk::enumerateInstanceLayerProperties();

		debugger_log("Available Vulkan instance layers ({}): ", layers.size());
		for (const auto& layer : layers)
			debugger_log("\t{}", layer.layerName);
	}

	bool check_instance_extensions(const std::vector<const char*>& reqs) {
		using scimitar::util::contains_if;

		auto available = vk::enumerateInstanceExtensionProperties();

		for (const auto& req : reqs) {
			if (!contains_if(available, [req](const vk::ExtensionProperties& props) {
				return std::string(props.extensionName.data()) == req;
			})) {
				std::cout << "Missing required instance extension: " << req << "\n";
				return false;
			}
		}

		return true;
	}

	bool check_instance_iayers(const std::vector<const char*>& reqs) {
		using scimitar::util::contains_if;

		auto available = vk::enumerateInstanceLayerProperties();

		for (const auto& req : reqs) {
			if (!contains_if(available, [req](const vk::LayerProperties& props) {
				return std::string(props.layerName.data()) == req;
			})) {
				std::cout << "Missing required instance layer: " << req << "\n";
				return false;
			}
		}

		return true;
	}
}

VkResult vkCreateDebugUtilsMessengerEXT(
	VkInstance                                instance, 
	const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
	const VkAllocationCallbacks*              allocator, 
	VkDebugUtilsMessengerEXT*                 messenger
) {
	static PFN_vkCreateDebugUtilsMessengerEXT impl_vkCreateDebugUtilsMessengerEXT = nullptr;
	if (!impl_vkCreateDebugUtilsMessengerEXT)
		impl_vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

	return impl_vkCreateDebugUtilsMessengerEXT(
		instance, 
		createInfo, 
		allocator, 
		messenger
	);
}
	
void vkDestroyDebugUtilsMessengerEXT(
	VkInstance                   instance, 
	VkDebugUtilsMessengerEXT     messenger, 
	const VkAllocationCallbacks* allocator
) {
	static PFN_vkDestroyDebugUtilsMessengerEXT impl_vkDestroyDebugUtilsMessengerEXT = nullptr;
	if (!impl_vkDestroyDebugUtilsMessengerEXT)
		impl_vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

	impl_vkDestroyDebugUtilsMessengerEXT(
		instance, 
		messenger, 
		allocator
	);
}

namespace scimitar {
	OS::OS():
		System("OS")
	{	
	}

	void OS::init() {
		System::init();

		list_instance_extensions();
		list_instance_layers();

		{
			vk::ApplicationInfo ai;
			ai.setApiVersion        (VK_API_VERSION_1_2);
			ai.setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));
			ai.setEngineVersion     (VK_MAKE_VERSION(0, 1, 0));
			ai.setPApplicationName  ("Scimitar application");
			ai.setPEngineName       ("Scimitar engine");

			m_RequiredInstanceExtensions = {
				VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
				VK_KHR_SURFACE_EXTENSION_NAME
			};

			if constexpr (ePlatform::current == ePlatform::windows) {
				m_RequiredInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			}

			if constexpr (
				(ePlatform::current == ePlatform::windows) &&
				(eBuild   ::current == eBuild   ::debug)
			) {
				m_RequiredInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				
				m_RequiredInstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
				// "VK_LAYER_LUNARG_api_dump"
			}
			
			if constexpr (eBuild::current == eBuild::debug) {
				m_RequiredDeviceFeatures.robustBufferAccess = VK_TRUE;
			}

			if (!check_instance_extensions(m_RequiredInstanceExtensions))
				throw std::runtime_error("Not all required Vulkan instance extensions are available");
			if (!check_instance_iayers(m_RequiredInstanceLayers))
				throw std::runtime_error("Not all required Vulkan instance layers are available");

			vk::InstanceCreateInfo ici;
			ici.setPApplicationInfo      (&ai);
			ici.setEnabledExtensionCount (static_cast<uint32_t>(m_RequiredInstanceExtensions.size()));
			ici.setPEnabledExtensionNames(m_RequiredInstanceExtensions);
			ici.setEnabledLayerCount     (static_cast<uint32_t>(m_RequiredInstanceLayers.size()));
			ici.setPEnabledLayerNames    (m_RequiredInstanceLayers);

			m_VkInstance = vk::createInstanceUnique(ici);
			m_VkLoader   = vk::DispatchLoaderDynamic(m_VkInstance.get(), vkGetInstanceProcAddr);
		}

		if constexpr (
			(ePlatform::current == ePlatform::windows) &&
			(eBuild   ::current == eBuild   ::debug)
		) {
			vk::DebugUtilsMessengerCreateInfoEXT info;

			info.setMessageSeverity(
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
				//vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo    | // spammy
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
			);

			info.setMessageType(
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral     |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
			);

			info.setPfnUserCallback(debug_callback);
			info.setPUserData(this);

			m_VkDebugMessager = m_VkInstance->createDebugUtilsMessengerEXTUnique(info);
		}

		// identify all available vulkan devices, provide RenderDevices for all of them
		for (auto physical : m_VkInstance->enumeratePhysicalDevices())
			m_RenderDevices.push_back(std::make_unique<RenderDevice>(this, physical));
	}

	void OS::update() {
	}

	void OS::shutdown() {
		System::shutdown();
	}

	const vk::Instance& OS::get_vk_instance() const noexcept {
		return m_VkInstance.get();
	}

	const vk::PhysicalDeviceFeatures& OS::get_vk_required_physical_features() const noexcept {
		return m_RequiredDeviceFeatures;
	}

	const vk::PhysicalDeviceLimits& OS::get_vk_required_physical_limits() const noexcept {
		return m_RequiredDeviceLimits;
	}

	VkBool32 OS::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
		VkDebugUtilsMessageTypeFlagsEXT             type,
		const VkDebugUtilsMessengerCallbackDataEXT* data,
		void*                                       userdata
	) {
		(void)type;
		(void)userdata;

		switch (severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			// debugger_log("Vulkan: {}", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: 
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: 
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: 
			debugger_log("Vulkan: {}", data->pMessage);
			break;

		default: 
			break;
		}

		return VK_FALSE;
	}
}