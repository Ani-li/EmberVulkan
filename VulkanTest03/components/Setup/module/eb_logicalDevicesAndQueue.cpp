#include "eb_logicalDevicesAndQueue.hpp"
#include "../eb_setup.hpp"

namespace eb {
	EbLogicalDevicesAndQueue::EbLogicalDevicesAndQueue(
		VkInstance instance,
		VkSurfaceKHR surface,
		uint32_t validationLayersCount,
		const char* const* validationLayersData,
		EbSetup* manager
	) :EbPhysicalDevicesAndQueue(instance, surface, validationLayersCount, validationLayersData), SetupManager(manager)
	{
		if (SetupManager) {
			SetupManager->Init_physicalDevice = physicalDevice;
			SetupManager->Init_graphicsFamily = queueFamilyIndices.graphicsFamily;
			SetupManager->Init_presentFamily = queueFamilyIndices.presentFamily;
		}
		createLogicalDevice();
	}
	EbLogicalDevicesAndQueue::~EbLogicalDevicesAndQueue() {
		vkDestroyDevice(device, nullptr);
	}

	void EbLogicalDevicesAndQueue::createLogicalDevice() {
		QueueFamilyIndices indices = queueFamilyIndices;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		auto extensions = getRequireExtensions();
		auto layers = getRequireLayers();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = layers.data();

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		if (SetupManager) {
			SetupManager->Init_device = device;
		}

		vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);

		if (SetupManager) {
			SetupManager->Init_graphicsQueue = graphicsQueue;
			SetupManager->Init_presentQueue = presentQueue;
		}
	}
}