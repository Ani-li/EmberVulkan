#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <set>
#include "eb_physicalDevicesAndQueue.hpp"

namespace eb {
	class EbSetup;

	class EbLogicalDevicesAndQueue : public EbPhysicalDevicesAndQueue
	{
	private:
		EbSetup* SetupManager;
	public:
		EbLogicalDevicesAndQueue(
			VkInstance instance,
			VkSurfaceKHR surface,
			uint32_t validationLayersCount,
			const char* const* validationLayersData,
			EbSetup* manager
		);
		~EbLogicalDevicesAndQueue();

	private:
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		void createLogicalDevice();
	};
}