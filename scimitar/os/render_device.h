#pragma once

#include "../dependencies.h"

namespace scimitar::os {
	class RenderDevice {
	public:
		RenderDevice(vk::PhysicalDevice physical);

		vk::Device getHandle(); // the logical one

	private:
		vk::PhysicalDevice                 m_Physical;
		vk::PhysicalDeviceProperties       m_PhysicalProperties;
		vk::PhysicalDeviceMemoryProperties m_PhysicalMemoryProperties;

		vk::UniqueDevice                   m_Logical;

	};
}