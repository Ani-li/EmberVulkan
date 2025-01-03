#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include "eb_uniformBufferCache.hpp"
#include "../Setup/eb_setup.hpp"

namespace eb
{
	class EbDescriptorSetCache
	{
	public:
		EbDescriptorSetCache(EbSetup& ebSetup);
		~EbDescriptorSetCache();

		void bindDescriptorSet(VkCommandBuffer cmd, VkPipelineLayout layout);
		VkDescriptorSetLayout* getVkDescriptorSetLayout();

	private:
		EbSetup& ebSetup;

		EbDescriptorPool::DescriptorSetData descriptorSetData;
		EbUniformBufferCache uniformBufferCache;

	public:
		void registerUniformBufferMember(const std::string& name, uint32_t size) { uniformBufferCache.registerUboMember(name, size); }
		void updateUniformBufferMember(const std::string& variableName, const void* data) { uniformBufferCache.updateUboMember(variableName, data); }
	};
}