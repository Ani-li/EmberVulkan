#include "eb_descriptorSetCache.hpp"

namespace eb
{
	EbDescriptorSetCache::EbDescriptorSetCache(EbSetup& ebSetup) 
		: 
		ebSetup(ebSetup),
		uniformBufferCache(ebSetup)
	{

	}

	EbDescriptorSetCache::~EbDescriptorSetCache()
	{
		vkDestroyDescriptorSetLayout(ebSetup.Init_device, descriptorSetData.layout, nullptr);
	}

	void EbDescriptorSetCache::BindDescriptorSet(VkCommandBuffer cmd, VkPipelineLayout layout)
	{
		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSetData.set, 0, nullptr);
	}

	VkDescriptorSetLayout* EbDescriptorSetCache::getVkDescriptorSetLayout()
	{
		auto uniformBuffer = uniformBufferCache.createUniformBuffer();
		std::vector<EbDescriptorPool::BindingData> bindings;

		auto binding = ebSetup.getEbDescriptorPool().createBindingData(
			0,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			&uniformBuffer,
			nullptr,
			nullptr
		);
		bindings.push_back(binding);
		descriptorSetData = ebSetup.getEbDescriptorPool().createDescriptorSetData(bindings);
		return &descriptorSetData.layout;
	}
}