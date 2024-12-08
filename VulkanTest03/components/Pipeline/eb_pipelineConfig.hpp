#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace eb
{
	struct PipelineConfigInfo
	{
		std::string vertFilepath;
		std::string fragFilepath;
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachment;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subPass;

		static PipelineConfigInfo defaultPipelineConfigInfo(
			uint32_t width,
			uint32_t height,
			std::string vertFilepath,
			std::string fragFilepath,
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachment,
			VkRenderPass renderPass,
			uint32_t subPassIdx
		);
	};
}