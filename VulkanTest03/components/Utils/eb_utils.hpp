#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstring>
#include <vector>
#include "../Setup/module/eb_commandPool.hpp"

namespace eb
{
	class EbUtils
	{
	public:
		static void createBuffer(
			VkDevice& device,
			VkPhysicalDevice& physicalDevice,
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory
		);

		static void createImage(
			VkDevice& device,
			VkPhysicalDevice& physicalDevice,
			uint32_t width,
			uint32_t height,
			uint32_t mipLevels,
			VkSampleCountFlagBits numSamples,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory
		);

		static VkFormat findSupportedFormat(
			VkPhysicalDevice& physicalDevice,
			const std::vector<VkFormat>& candidates,
			VkImageTiling tiling,
			VkFormatFeatureFlags features
		);

		static void copyBuffer(
			EbCommandPool& commandPool,
			VkBuffer srcBuffer,
			VkBuffer dstBuffer,
			VkDeviceSize size
		);

		static void copyBufferToImage(
			EbCommandPool& commandPool,
			VkBuffer buffer,
			VkImage image,
			uint32_t width,
			uint32_t height
		);

		static VkImageView createImageView(
			VkDevice& device,
			VkImage image,
			VkFormat format,
			VkImageAspectFlags aspectFlags,
			uint32_t mipLevels
		);

		static float CalAspectRatio(
			uint32_t width,
			uint32_t height
		);

	private:
		static uint32_t findMemoryType(
			VkPhysicalDevice& physicalDevice,
			uint32_t typeFilter,
			VkMemoryPropertyFlags properties
		);
	};
}