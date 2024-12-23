#include "eb_descriptorPool.hpp"
#include "../eb_setup.hpp"

namespace eb
{
	EbDescriptorPool::EbDescriptorPool(
		VkDevice device,
		EbSetup* manager
	):SetupManager(manager),device(device)
	{
		createDescriptorPool();
	}

	EbDescriptorPool::~EbDescriptorPool()
	{
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	}

	EbDescriptorPool::BindingData EbDescriptorPool::createBindingData(
		uint32_t bindingSite,
		VkDescriptorType type,
		VkDescriptorBufferInfo* bufferInfo,
		VkDescriptorImageInfo* imageInfo,
		VkBufferView* texelBufferInfo
	)
	{
		BindingData outData{};
		outData.bind.binding = bindingSite;
		outData.bind.descriptorType = type;
		outData.bind.descriptorCount = 1;
		outData.bind.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
		outData.bind.pImmutableSamplers = nullptr;

		outData.write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		outData.write.dstSet = nullptr;
		outData.write.dstBinding = bindingSite;
		outData.write.dstArrayElement = 0;
		outData.write.descriptorType = type;
		outData.write.descriptorCount = 1;
		outData.write.pBufferInfo = bufferInfo;
		outData.write.pImageInfo = imageInfo;
		outData.write.pTexelBufferView = texelBufferInfo;

		return outData;
	}

	EbDescriptorPool::DescriptorSetData EbDescriptorPool::createDescriptorSetData(
		std::vector<EbDescriptorPool::BindingData> bindingInfo
	)
	{
		std::vector<VkDescriptorSetLayoutBinding> bingings;
		for (const auto &var : bindingInfo)
		{
			bingings.push_back(var.bind);
		}

		DescriptorSetData outData{};

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bingings.size();
		layoutInfo.pBindings = bingings.data();
		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &outData.layout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &outData.layout;
		if (vkAllocateDescriptorSets(device, &allocInfo, &outData.set) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (auto& var : bindingInfo)
		{
			var.write.dstSet = outData.set;
			vkUpdateDescriptorSets(device, 1, &var.write, 0, nullptr);
		}

		return outData;
	}

	void EbDescriptorPool::createDescriptorPool()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = 1;

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
		if (SetupManager)
		{
			SetupManager->Init_descriptorPool = descriptorPool;
		}
	}
}