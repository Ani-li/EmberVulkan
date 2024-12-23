#include "eb_renderPassFactory.hpp"

namespace eb
{
	EbRenderPassFactory::EbRenderPassFactory(
		EbSetup& ebSetup,
		std::string vertFilepath,
		std::string fragFilepath,
		std::string name,
		glm::vec4 debugColor
	) : ebSetup(ebSetup),
		renderPassDebugName(name),
		renderPassDebugColor(debugColor),
		vertFilepath(vertFilepath),
		fragFilepath(fragFilepath),
		ebModelCache(ebSetup),
		ebPushConstantCache(ebSetup),
		ebDescriptorCache(ebSetup)
	{

	}

	EbRenderPassFactory::~EbRenderPassFactory()
	{
		for (auto var : frameBuffer) {
			vkDestroyFramebuffer(ebSetup.Init_device, var, nullptr);
		}
		vkDestroyPipelineLayout(ebSetup.Init_device, pipelineConfigInfo.pipelineLayout, nullptr);
		vkDestroyRenderPass(ebSetup.Init_device, renderPass, nullptr);
	}

	void EbRenderPassFactory::createRenderPass()
	{
		createRenderPassSettings(colorAttachmentCount, colorAttachment, colorBlendAttachment, depthStencilAttachmentCount, depthStencilAttachment);

		assert(colorAttachmentCount == colorAttachment.size());
		assert(colorAttachmentCount == colorBlendAttachment.size());

		std::vector<VkAttachmentDescription> AttachmentDescriptions;
		std::vector<VkAttachmentReference> colorAttachmentReferences;
		for (auto var : colorAttachment)
		{
			AttachmentDescriptions.push_back(var.description);
			colorAttachmentReferences.push_back(var.reference);
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		if (colorAttachment.size() > 0)
		{
			subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachment.size());
			subpass.pColorAttachments = colorAttachmentReferences.data();
		}

		if (depthStencilAttachmentCount == 1)
		{
			AttachmentDescriptions.push_back(depthStencilAttachment.description);
			subpass.pDepthStencilAttachment = &depthStencilAttachment.reference;
		}

		VkSubpassDependency depend{};
		depend.srcSubpass = VK_SUBPASS_EXTERNAL;
		depend.dstSubpass = 0;

		depend.srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		depend.dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

		depend.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
		depend.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(AttachmentDescriptions.size());
		renderPassInfo.pAttachments = AttachmentDescriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &depend;

		if (vkCreateRenderPass(ebSetup.Init_device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void EbRenderPassFactory::createFrameBuffer()
	{
		setFrameBufferAttachment(frameBufferAttachments);

		assert(static_cast<uint32_t>(frameBufferAttachments.size()) == ebSetup.Init_swapChainImageCount);
		assert(static_cast<uint32_t>(frameBufferAttachments[0].size()) == colorAttachmentCount + depthStencilAttachmentCount);
		frameBuffer.resize(ebSetup.Init_swapChainImageCount);
		for (size_t i = 0; i < ebSetup.Init_swapChainImageCount; i++) {
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(frameBufferAttachments[i].size());
			framebufferInfo.pAttachments = frameBufferAttachments[i].data();
			framebufferInfo.width = ebSetup.Init_swapChainExtent.width;
			framebufferInfo.height = ebSetup.Init_swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(ebSetup.Init_device, &framebufferInfo, nullptr, &frameBuffer[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void EbRenderPassFactory::beginRenderPass(
		VkCommandBuffer commandBuffer,
		uint32_t currentImageIndex,
		VkClearColorValue clearColorValue,
		VkClearDepthStencilValue clearDepthStencilValue
	)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = frameBuffer[currentImageIndex];

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = ebSetup.Init_swapChainExtent;

		std::vector<VkClearValue> clearValues;
		for (uint32_t i = 0; i < colorAttachmentCount; i++)
		{
			VkClearValue var;
			var.color = clearColorValue;
			clearValues.push_back(var);
		}
		for (uint32_t i = 0; i < depthStencilAttachmentCount; i++)
		{
			VkClearValue var;
			var.depthStencil = clearDepthStencilValue;
			clearValues.push_back(var);
		}
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT =
			(PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(ebSetup.Init_vkInstance, "vkCmdBeginDebugUtilsLabelEXT");

		VkDebugUtilsLabelEXT labelStart = {};
		labelStart.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		labelStart.pLabelName = renderPassDebugName.c_str();
		labelStart.color[0] = renderPassDebugColor[0];
		labelStart.color[1] = renderPassDebugColor[1];
		labelStart.color[2] = renderPassDebugColor[2];
		labelStart.color[3] = renderPassDebugColor[3];

		vkCmdBeginDebugUtilsLabelEXT(commandBuffer, &labelStart);

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		cmd = commandBuffer;

		ebDescriptorCache.BindDescriptorSet(commandBuffer, pipelineConfigInfo.pipelineLayout);
	}

	void EbRenderPassFactory::endRenderPass(VkCommandBuffer commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);

		PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT =
			(PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(ebSetup.Init_vkInstance, "vkCmdEndDebugUtilsLabelEXT");

		vkCmdEndDebugUtilsLabelEXT(commandBuffer);
	}

	void EbRenderPassFactory::Init()
	{
		createRenderPass();
		createFrameBuffer();
		registerModel(ebModelCache);
		registerConstantValue(ebPushConstantCache);
		registerDescriptorValue(ebDescriptorCache);
		createPipelineConfigInfo();
		createPipelineLayout();
		overridePipelineConfigInfo(pipelineConfigInfo);
	}

	void EbRenderPassFactory::createPipelineConfigInfo()
	{
		PipelineConfigInfo info = PipelineConfigInfo::defaultPipelineConfigInfo(
			ebSetup.Init_swapChainExtent.width,
			ebSetup.Init_swapChainExtent.height,
			vertFilepath,
			fragFilepath,
			colorBlendAttachment,
			renderPass,
			0
		);
		pipelineConfigInfo = info;
	}

	void EbRenderPassFactory::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = ebPushConstantCache.getCurrentSize();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = ebDescriptorCache.getVkDescriptorSetLayout();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(ebSetup.Init_device, &pipelineLayoutInfo, nullptr, &pipelineConfigInfo.pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	EbModel EbRenderPassFactory::getModel(const std::string& name)
	{
		return ebModelCache.getModel(name);
	}

	void EbRenderPassFactory::updateConstantValue(const std::string& variableName, const void* data)
	{
		if (cmd == nullptr || pipelineConfigInfo.pipelineLayout == nullptr)
		{
			throw std::runtime_error("render pass has not been initialized!");
		}
		ebPushConstantCache.updateConstantValue(cmd, pipelineConfigInfo.pipelineLayout, variableName, data);
	}

	void EbRenderPassFactory::updateDescriptorValue(const std::string& variableName, const void* data)
	{
		ebDescriptorCache.updateUniformBufferMember(variableName, data);
	}
}