#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>

namespace eb {
	class EbPhysicalDevicesAndQueue
	{
	public:
		EbPhysicalDevicesAndQueue(
			VkInstance instance,
			VkSurfaceKHR surface,
			uint32_t validationLayersCount,
			const char* const* validationLayersData
		);
		~EbPhysicalDevicesAndQueue();

	protected:
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		QueueFamilyIndices queueFamilyIndices;
		std::vector<const char*> getRequireLayers();
		std::vector<const char*> getRequireExtensions();

	private:
		uint32_t ValiationLayersCount;
		const char* const* ValiationLayersData;

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	};



}