#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>

namespace eb {
	class EbSetup;

	class EbCommandPool
	{
	private:
		EbSetup* SetupManager;

	public:
		EbCommandPool(
			VkDevice device,
			std::optional<uint32_t> graphicsFamily,
			VkQueue graphicsQueue,
			EbSetup* manager
		);
		~EbCommandPool();

		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	private:
		VkDevice device;
		VkQueue graphicsQueue;
		VkCommandPool commandPool;

		void createCommandPool(std::optional<uint32_t> graphicsFamily);
	};
}