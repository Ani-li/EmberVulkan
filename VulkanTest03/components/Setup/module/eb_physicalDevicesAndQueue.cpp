#include "eb_physicalDevicesAndQueue.hpp"
#include "iostream"

namespace eb {
	EbPhysicalDevicesAndQueue::EbPhysicalDevicesAndQueue(
		VkInstance instance,
		VkSurfaceKHR surface,
		uint32_t validationLayersCount,
		const char* const* validationLayersData
	) : ValiationLayersCount(validationLayersCount), ValiationLayersData(validationLayersData)
	{
		pickPhysicalDevice(instance, surface);
	}

	EbPhysicalDevicesAndQueue::~EbPhysicalDevicesAndQueue() {}

	void EbPhysicalDevicesAndQueue::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with vulkan support!");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device, surface)) {
				physicalDevice = device;
				printPhysicalDeviceInfo(device);
				queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	bool EbPhysicalDevicesAndQueue::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = findQueueFamilies(device, surface);

		bool extensionsSupported = checkDeviceExtensionSupport(device);
		bool swapChainAdequate = false;
		bool pushConstantMaxSizeAdequate = false;

		if (extensionsSupported) {
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

			swapChainAdequate = !(formatCount == 0) && !(presentModeCount == 0);

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			pushConstantMaxSizeAdequate = deviceProperties.limits.maxPushConstantsSize >= 256;
		}

		return indices.graphicsFamily.has_value() && extensionsSupported && swapChainAdequate && pushConstantMaxSizeAdequate;
	}

	bool EbPhysicalDevicesAndQueue::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	EbPhysicalDevicesAndQueue::QueueFamilyIndices EbPhysicalDevicesAndQueue::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}
		return indices;
	}

	std::vector<const char*> EbPhysicalDevicesAndQueue::getRequireLayers()
	{
		std::vector<const char*> layers;
		for (uint32_t i = 0; i < ValiationLayersCount; i++) {
			layers.push_back(ValiationLayersData[i]);
		}
		return layers;
	}
	std::vector<const char*> EbPhysicalDevicesAndQueue::getRequireExtensions()
	{
		std::vector<const char*> extensions;

		for (uint32_t i = 0; i < deviceExtensions.size(); i++) {
			extensions.push_back(deviceExtensions[i]);
		}

		return extensions;
	}

	void EbPhysicalDevicesAndQueue::printPhysicalDeviceInfo(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		std::cout << "_______________Current Device______________" << std::endl;
		std::cout << "___________________________________________" << std::endl;
		std::cout << "Device Name: " << deviceProperties.deviceName << std::endl;
		std::cout << "API Version: " << VK_VERSION_MAJOR(deviceProperties.apiVersion)
			<< "." << VK_VERSION_MINOR(deviceProperties.apiVersion)
			<< "." << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
		std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
		std::cout << "Vendor ID: " << deviceProperties.vendorID << std::endl;
		std::cout << "Device ID: " << deviceProperties.deviceID << std::endl;
		std::cout << "___________________________________________" << std::endl;
		std::cout << " " << std::endl;
	}
}