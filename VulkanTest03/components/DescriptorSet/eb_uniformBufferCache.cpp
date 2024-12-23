#include "eb_uniformBufferCache.hpp"

namespace eb
{
	EbUniformBufferCache::EbUniformBufferCache(EbSetup& ebSetup):ebSetup(ebSetup)
	{

	}

	EbUniformBufferCache::~EbUniformBufferCache()
	{
		vkDestroyBuffer(ebSetup.Init_device, uniformBuffer, nullptr);
		vkFreeMemory(ebSetup.Init_device, uniformMemory, nullptr);
	}

	VkDescriptorBufferInfo EbUniformBufferCache::createUniformBuffer()
	{
		EbUtils::createBuffer(
			ebSetup.Init_device,
			ebSetup.Init_physicalDevice,
			currentUniformBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffer,
			uniformMemory
		);
		vkMapMemory(ebSetup.Init_device, uniformMemory, 0, currentUniformBufferSize, 0, &uniformBufferMapped);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = currentUniformBufferSize;

		return bufferInfo;
	}

	void EbUniformBufferCache::registerUboMember(const std::string& name, uint32_t size)
	{
		uint32_t offset = (currentUniformBufferSize + 15) & ~15; // ¶ÔÆëµ½ 16 ×Ö½Ú
		uboMemberMap[name] = { offset, size };
		currentUniformBufferSize = offset + size;
	}

	void EbUniformBufferCache::updateUboMember(const std::string& variableName, const void* data)
	{
		auto it = uboMemberMap.find(variableName);
		if (it == uboMemberMap.end()) {
			throw std::runtime_error("Variable not found in Uniform Buffer");
		}

		const uboMemberInfo& info = it->second;
		memcpy(static_cast<uint8_t*>(uniformBufferMapped) + info.offset, data, info.size);
	}
}