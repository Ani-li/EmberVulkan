#include "eb_pushConstantCache.hpp"
#include <stdexcept>

namespace eb
{
	EbPushConstantCache::EbPushConstantCache(EbSetup& ebSetup) :ebSetup(ebSetup)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(ebSetup.Init_physicalDevice, &deviceProperties);
		maxSize = deviceProperties.limits.maxPushConstantsSize;
	}

	void EbPushConstantCache::registerConstantValue(const std::string& name, uint32_t size)
	{
		uint32_t offset = (currentSize + 15) & ~(15);
		if (offset > maxSize) {
			throw std::runtime_error("the size of push Constants outweight the maxPushConstantsSize!");
		}
		constantMap[name] = { offset, size };
		currentSize = offset + size;
	}

	void EbPushConstantCache::updateConstantValue(
		VkCommandBuffer commandBuffer,
		VkPipelineLayout pipelineLayout,
		const std::string& variableName,
		const void* data
	)
	{
		auto it = constantMap.find(variableName);
		if (it == constantMap.end()) {
			throw std::runtime_error("Variable not found in Push Constants");
		}
		const constantSize& info = it->second;

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, // 可见阶段
			info.offset,                                               // 偏移
			info.size,                                                 // 大小
			data                                                       // 数据指针
		);
	}

}