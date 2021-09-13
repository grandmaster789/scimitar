#include "render_device.h"
#include "../core/logger.h"
#include <sstream>

namespace scimitar::os {
	RenderDevice::RenderDevice(vk::PhysicalDevice physical) :
		m_Physical(physical)
	{
		// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#devsandqueues-physical-device-enumeration
		// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#VkPhysicalDeviceProperties2
		m_PhysicalProperties = physical.getProperties();

		std::stringstream sstr;

		int api_major = VK_VERSION_MAJOR(m_PhysicalProperties.apiVersion);
		int api_minor = VK_VERSION_MINOR(m_PhysicalProperties.apiVersion);
		int api_patch = VK_VERSION_PATCH(m_PhysicalProperties.apiVersion);

		int drv_major = VK_VERSION_MAJOR(m_PhysicalProperties.driverVersion);
		int drv_minor = VK_VERSION_MINOR(m_PhysicalProperties.driverVersion);
		int drv_patch = VK_VERSION_PATCH(m_PhysicalProperties.driverVersion);

		sstr
			<< "RenderDevice: " << m_PhysicalProperties.deviceName << "\n"
			<< "\tAPI:    " << api_major << "." << api_minor << "." << api_patch << "\n"
			<< "\tDriver: " << drv_major << "." << drv_minor << "." << drv_patch;

		gLog << sstr.str();


	}

	vk::Device RenderDevice::getHandle() {
		return m_Logical.get();
	}
}