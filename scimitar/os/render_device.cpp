#include "render_device.h"
#include "../core/logger.h"
#include <sstream>

namespace scimitar::os {
	RenderDevice::RenderDevice(vk::PhysicalDevice physical) :
		m_Physical(physical)
	{
		// TODO: determine the full capability set of the physical device
		// so the getProperties2 yields a sizable linked list of different device properties that require
		// a fair amount of dynamic inspection to fully parse... currently I'm not *that* concerned with
		// determining the full capability set of the hardware so I'll leave it as a todo
		m_PhysicalProperties       = physical.getProperties2().properties;
		m_PhysicalMemoryProperties = physical.getMemoryProperties();

		std::stringstream sstr;

		int api_major = VK_VERSION_MAJOR(m_PhysicalProperties.apiVersion);
		int api_minor = VK_VERSION_MINOR(m_PhysicalProperties.apiVersion);
		int api_patch = VK_VERSION_PATCH(m_PhysicalProperties.apiVersion);

		int drv_major = VK_VERSION_MAJOR(m_PhysicalProperties.driverVersion);
		int drv_minor = VK_VERSION_MINOR(m_PhysicalProperties.driverVersion);
		int drv_patch = VK_VERSION_PATCH(m_PhysicalProperties.driverVersion);
				
		sstr
			<< "RenderDevice: " << m_PhysicalProperties.deviceName << "\n"
			<< "Vulkan API:   " << api_major << "." << api_minor << "." << api_patch << "\n"
			<< "Driver: "       << " v" << drv_major << "." << drv_minor << "." << drv_patch << "\n";

		gLog << sstr.str();


	}

	vk::Device RenderDevice::getHandle() {
		return m_Logical.get();
	}
}