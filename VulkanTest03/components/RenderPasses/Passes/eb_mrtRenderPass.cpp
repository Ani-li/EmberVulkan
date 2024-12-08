#include "eb_mrtRenderPass.hpp"

namespace eb
{
	EbMrtRenderPass::EbMrtRenderPass(EbSetup& ebSetup) : EbRenderPassFactory(
		ebSetup,
		"F:\\Project\\GraphicsProject\\VulkanTest03\\VulkanTest03\\shaders\\simple_shader_vert.spv",
		"F:\\Project\\GraphicsProject\\VulkanTest03\\VulkanTest03\\shaders\\simple_shader_frag.spv",
		"Mrt Pass",
		glm::vec4(0, 1, 0, 1)
	), ebSetup(ebSetup)
	{
		createDepthResources();
		createNormalResource();
	}

	EbMrtRenderPass::~EbMrtRenderPass()
	{
		vkDestroyImageView(ebSetup.Init_device, depthImageView, nullptr);
		vkDestroyImage(ebSetup.Init_device, depthImage, nullptr);
		vkFreeMemory(ebSetup.Init_device, depthImageMemory, nullptr);
		vkDestroyImageView(ebSetup.Init_device, normalImageView, nullptr);
		vkDestroyImage(ebSetup.Init_device, normalImage, nullptr);
		vkFreeMemory(ebSetup.Init_device, normalImageMemory, nullptr);
	}

	void EbMrtRenderPass::createRenderPassSettings(
		uint32_t& colorAttachmentCount,
		std::vector<RenderPassAttachment>& colorAttachment,
		std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachment,
		uint32_t& depthStencilAttachmentCount,
		RenderPassAttachment& depthStencilAttachment
	)
	{
		RenderPassAttachment color_1;
		color_1.description.flags = 0;
		color_1.description.format = VK_FORMAT_B8G8R8A8_SRGB;
		color_1.description.samples = VK_SAMPLE_COUNT_1_BIT;
		color_1.description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_1.description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_1.description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_1.description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_1.description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_1.description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		color_1.reference.attachment = 0;
		color_1.reference.layout = VK_IMAGE_LAYOUT_GENERAL;

		RenderPassAttachment color_2;
		color_2.description.flags = 0;
		color_2.description.format = VK_FORMAT_B8G8R8A8_SRGB;
		color_2.description.samples = VK_SAMPLE_COUNT_1_BIT;
		color_2.description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_2.description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_2.description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_2.description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_2.description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_2.description.finalLayout = VK_IMAGE_LAYOUT_GENERAL;

		color_2.reference.attachment = 1;
		color_2.reference.layout = VK_IMAGE_LAYOUT_GENERAL;

		colorAttachmentCount = 2;
		colorAttachment.clear();
		colorAttachment.push_back(color_1);
		colorAttachment.push_back(color_2);

		VkPipelineColorBlendAttachmentState blend_1{};
		blend_1.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blend_1.blendEnable = VK_FALSE;
		blend_1.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blend_1.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blend_1.colorBlendOp = VK_BLEND_OP_ADD;
		blend_1.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blend_1.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blend_1.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendAttachmentState blend_2{};
		blend_2.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blend_2.blendEnable = VK_FALSE;
		blend_2.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blend_2.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blend_2.colorBlendOp = VK_BLEND_OP_ADD;
		blend_2.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blend_2.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blend_2.alphaBlendOp = VK_BLEND_OP_ADD;

		colorBlendAttachment.clear();
		colorBlendAttachment.push_back(blend_1);
		colorBlendAttachment.push_back(blend_2);

		RenderPassAttachment depth;
		depth.description.flags = 0;
		depth.description.format = VK_FORMAT_D32_SFLOAT;
		depth.description.samples = VK_SAMPLE_COUNT_1_BIT;
		depth.description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth.description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth.description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depth.description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth.description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth.description.finalLayout = VK_IMAGE_LAYOUT_GENERAL;

		depth.reference.attachment = 2;
		depth.reference.layout = VK_IMAGE_LAYOUT_GENERAL;

		depthStencilAttachmentCount = 1;
		depthStencilAttachment = depth;
	}

	void EbMrtRenderPass::setFrameBufferAttachment(std::vector<std::vector<VkImageView>>& frameBufferAttachments)
	{
		for (size_t i = 0; i < ebSetup.Init_swapChainImageViews.size(); i++)
		{
			std::vector<VkImageView> att;
			att.push_back(ebSetup.Init_swapChainImageViews[i]);
			att.push_back(normalImageView);
			att.push_back(depthImageView);
			frameBufferAttachments.push_back(att);
		}
	}

	void EbMrtRenderPass::registerModel(EbModelCache& ebModelCache)
	{
		ebModelCache.registerModel_file("F:\\Project\\GraphicsProject\\VulkanTest03\\VulkanTest03\\resources\\models\\smooth_vase.obj", "obj1");
		ebModelCache.registerModel_file("F:\\Project\\GraphicsProject\\VulkanTest03\\VulkanTest03\\resources\\models\\quad.obj", "obj2");
	}

	void EbMrtRenderPass::registerConstantValue(EbPushConstantCache& ebPushConstantCache)
	{
		ebPushConstantCache.registerConstantValue("projectionMatrix", sizeof(glm::mat4));
		ebPushConstantCache.registerConstantValue("viewMatrix", sizeof(glm::mat4));
		ebPushConstantCache.registerConstantValue("modelMatrix", sizeof(glm::mat4));
		ebPushConstantCache.registerConstantValue("mainLightDirection", sizeof(glm::vec3));
		ebPushConstantCache.registerConstantValue("mainLightColor", sizeof(glm::vec4));
		ebPushConstantCache.registerConstantValue("pointLightPosition", sizeof(glm::vec3));
		ebPushConstantCache.registerConstantValue("pointLightColor", sizeof(glm::vec4));
	}

	void EbMrtRenderPass::execute(VkCommandBuffer commandBuffer, EbCamera& camera)
	{
		
		updateConstantValue("projectionMatrix", &camera.getProjection());
		updateConstantValue("viewMatrix", &camera.getView());
		
		glm::vec3 mainLightDir = glm::normalize(glm::vec3(1.0, -3.0, -1.0));
		glm::vec4 mainLightCol = { 1.0f,1.0f,1.0f, 1.0f };
		glm::vec3 pointLightPos = { 2.0f,-1.2f,0.0f };
		glm::vec4 pointLightCol = { 1.0f,1.0f,1.0f, 1.0f };
		updateConstantValue("mainLightDirection", &mainLightDir);
		updateConstantValue("mainLightColor", &mainLightCol);
		updateConstantValue("pointLightPosition", &pointLightPos);
		updateConstantValue("pointLightColor", &pointLightCol);

		auto obj_1 = getModel("obj1");
		obj_1.transform.scale = { 2.0f,2.0f,2.0f };
		glm::mat4 mM = obj_1.transform.mat4();
		updateConstantValue("modelMatrix", &mM);
		obj_1.draw(commandBuffer);

		obj_1.transform.translation = { 0.3f,0.0f,0.2f };
		obj_1.transform.scale = { 1.5f,1.5f,1.5f };
		obj_1.transform.rotation = { 0,30,0 };
		mM = obj_1.transform.mat4();
		updateConstantValue("modelMatrix", &mM);
		obj_1.draw(commandBuffer);

		obj_1.transform.translation = { 0.4f,0.0f,-0.2f };
		obj_1.transform.scale = { 1.2f,1.2f,1.2f };
		obj_1.transform.rotation = { 0,149,0 };
		mM = obj_1.transform.mat4();
		updateConstantValue("modelMatrix", &mM);
		obj_1.draw(commandBuffer);

		auto obj_2 = getModel("obj2");
		mM = obj_2.transform.mat4();
		updateConstantValue("modelMatrix", &mM);
		obj_2.draw(commandBuffer);
	}

	void EbMrtRenderPass::createDepthResources()
	{
		VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
		EbUtils::createImage(ebSetup.Init_device, ebSetup.Init_physicalDevice, ebSetup.Init_swapChainExtent.width, ebSetup.Init_swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
		depthImageView = EbUtils::createImageView(ebSetup.Init_device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}

	void EbMrtRenderPass::createNormalResource()
	{
		VkFormat normalFormat = VK_FORMAT_B8G8R8A8_SRGB;
		EbUtils::createImage(
			ebSetup.Init_device,
			ebSetup.Init_physicalDevice,
			ebSetup.Init_swapChainExtent.width,
			ebSetup.Init_swapChainExtent.height,
			1,
			VK_SAMPLE_COUNT_1_BIT,
			normalFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			normalImage,
			normalImageMemory
		);
		normalImageView = EbUtils::createImageView(ebSetup.Init_device, normalImage, normalFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	REGISTER_RENDERPASS(EbMrtRenderPass, 1);
}