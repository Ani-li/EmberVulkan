#pragma once
#include "../eb_renderPassFactory.hpp"

namespace eb
{
	class EbMrtRenderPass : public EbRenderPassFactory
	{
	public:
		EbMrtRenderPass(EbSetup& ebSetup);
		~EbMrtRenderPass();

		void createRenderPassSettings(
			uint32_t& colorAttachmentCount,
			std::vector<RenderPassAttachment>& colorAttachment,
			std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachment,
			uint32_t& depthStencilAttachmentCount,
			RenderPassAttachment& depthStencilAttachment
		) override;


		void setFrameBufferAttachment(std::vector<std::vector<VkImageView>>& frameBufferAttachments) override;

		void registerModel(EbModelCache& ebModelCache) override;

		void registerConstantValue(EbPushConstantCache& ebPushConstantCache) override;

		void execute(VkCommandBuffer commandBuffer, EbCamera& camera) override;

	private:
		EbSetup& ebSetup;
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		VkImage normalImage;
		VkDeviceMemory normalImageMemory;
		VkImageView normalImageView;

		void createDepthResources();

		void createNormalResource();
	};
}