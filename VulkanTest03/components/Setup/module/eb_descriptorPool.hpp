#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>

namespace eb
{
	class EbSetup;

	class EbDescriptorPool
	{
	private:
		EbSetup* SetupManager;

	public:
		struct BindingData
		{
			VkDescriptorSetLayoutBinding bind;
			VkWriteDescriptorSet write;
		};

		struct DescriptorSetData
		{
			VkDescriptorSetLayout layout;
			VkDescriptorSet set;
		};

		EbDescriptorPool(
			VkDevice device,
			EbSetup* manager
		);
		~EbDescriptorPool();

		BindingData createBindingData(
			uint32_t bindingSite,
			VkDescriptorType type,
			VkDescriptorBufferInfo* bufferInfo,
			VkDescriptorImageInfo* imageInfo,
			VkBufferView* texelBufferInfo
		);

		DescriptorSetData createDescriptorSetData(
			std::vector<BindingData> bindingInfo
		);

	private:
		VkDevice device;

		VkDescriptorPool descriptorPool;

		void createDescriptorPool();
	};
}
