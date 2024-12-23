#pragma once
#include <unordered_map>
#include <string>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include "../Setup/eb_setup.hpp"

namespace eb
{
	class EbUniformBufferCache
	{
	public:
		EbUniformBufferCache(EbSetup& ebSetup);
		~EbUniformBufferCache();

		struct uboMemberInfo
		{
			uint32_t offset;
			uint32_t size;
		};

		VkDescriptorBufferInfo createUniformBuffer();

		void registerUboMember(const std::string& name, uint32_t size);

		void updateUboMember(const std::string& variableName, const void* data);

	private:
		EbSetup& ebSetup;

		uint32_t currentUniformBufferSize = 0;
		std::unordered_map<std::string, uboMemberInfo> uboMemberMap{};
		VkBuffer uniformBuffer;
		VkDeviceMemory uniformMemory;
		void* uniformBufferMapped;
	};
}