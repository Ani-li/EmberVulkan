#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <vulkan/vulkan.h>
#include "../Setup/eb_setup.hpp"

namespace eb
{
	class EbPushConstantCache
	{

	public:
		EbPushConstantCache(EbSetup& ebSetup);

		struct constantSize
		{
			uint32_t offset;
			uint32_t size;
		};

		void registerConstantValue(
			const std::string& name,
			uint32_t size
		);

		void updateConstantValue(
			VkCommandBuffer commandBuffer,
			VkPipelineLayout pipelineLayout,
			const std::string& variableName,
			const void* data
		);

		uint32_t getCurrentSize() { return currentSize; }

	private:
		EbSetup& ebSetup;

		std::unordered_map<std::string, constantSize> constantMap{};
		uint32_t currentSize = 0;
		uint32_t maxSize = 0;
	};
}