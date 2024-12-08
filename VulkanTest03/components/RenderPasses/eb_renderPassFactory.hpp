#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <cassert>
#include <map>
#include <functional>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "../Setup/eb_setup.hpp"
#include "../Utils/eb_utils.hpp"
#include "../Pipeline/eb_pipelineConfig.hpp"
#include "../Model/eb_modelCache.hpp"
#include "../PushConstant/eb_pushConstantCache.hpp"
#include "../Camera/eb_camera.hpp"

namespace eb
{
	class EbRenderPassFactory
	{
	public:
		EbRenderPassFactory(
			EbSetup& ebSetup,
			std::string vertFilepath,
			std::string fragFilepath,
			std::string name = "DefaultRenderPass",
			glm::vec4 debugColor = glm::vec4(0, 0, 0, 1)
		);

		virtual ~EbRenderPassFactory();

		//getValue
		EbSetup& getSetup() { return ebSetup; }
		PipelineConfigInfo getPipelineConfigInfo() { return pipelineConfigInfo; }

		//execute
		virtual void execute(VkCommandBuffer commandBuffer, EbCamera& camera) = 0;

		//other
		void Init();
		void beginRenderPass(
			VkCommandBuffer commandBuffer,
			uint32_t currentImageIndex,
			VkClearColorValue clearColorValue,
			VkClearDepthStencilValue clearDepthStencilValue
		);
		void endRenderPass(VkCommandBuffer commandBuffer);

	protected:
		//RenderPass
		struct RenderPassAttachment
		{
			VkAttachmentDescription description;
			VkAttachmentReference reference;
		};
		virtual void createRenderPassSettings(
			uint32_t& colorAttachmentCount,
			std::vector<RenderPassAttachment>& colorAttachment,
			std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachment,
			uint32_t& depthStencilAttachmentCount,
			RenderPassAttachment& depthStencilAttachment
		) = 0;

		//frameBuffer
		virtual void setFrameBufferAttachment(std::vector<std::vector<VkImageView>>& frameBufferAttachments) = 0;

		//pipelinConfig
		virtual void overridePipelineConfigInfo(PipelineConfigInfo& pipelineConfigInfo) {};

		//model
		virtual void registerModel(EbModelCache& ebModelCache) {};
		EbModel getModel(const std::string& name);

		//pushConstant
		virtual void registerConstantValue(EbPushConstantCache& ebPushConstantCache) {};
		void updateConstantValue(const std::string& variableName, const void* data);

	private:
		EbSetup& ebSetup;
		VkCommandBuffer cmd;
		EbModelCache ebModelCache;
		EbPushConstantCache ebPushConstantCache;

		VkRenderPass renderPass;
		std::vector<VkFramebuffer> frameBuffer;
		std::string vertFilepath;
		std::string fragFilepath;
		std::string renderPassDebugName;
		glm::vec4 renderPassDebugColor;
		uint32_t colorAttachmentCount;
		uint32_t depthStencilAttachmentCount;
		std::vector<RenderPassAttachment> colorAttachment;
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachment;
		RenderPassAttachment depthStencilAttachment;
		std::vector<std::vector<VkImageView>> frameBufferAttachments;
		PipelineConfigInfo pipelineConfigInfo;

		void createRenderPass();
		void createFrameBuffer();
		void createPipelineConfigInfo();
		void createPipelineLayout();

	private:
		using Registry = std::map<int, std::function<std::shared_ptr<EbRenderPassFactory>(EbSetup&)>>;

		static Registry& getRegistry()
		{
			static Registry registry;
			return registry;
		}

	public:
		static void registerClass(int order, std::function<std::shared_ptr<EbRenderPassFactory>(EbSetup&)> creator)
		{
			getRegistry()[order] = creator;
		}

		static std::vector<std::shared_ptr<EbRenderPassFactory>> getAllRenderPasses(EbSetup& setup)
		{
			std::vector<std::shared_ptr<EbRenderPassFactory>> result;
			for (const auto& [order, creator] : getRegistry())
			{
				result.push_back(creator(setup));
			}
			return result;
		}
	};

#define REGISTER_RENDERPASS(ClassType,Order) \
	struct ClassType##Register { \
		ClassType##Register(){ \
			EbRenderPassFactory::registerClass(Order, [](EbSetup& param) -> std::shared_ptr<EbRenderPassFactory> { \
				return std::make_shared<ClassType>(param); \
			}); \
		} \
	}; \
	static ClassType##Register global_##ClassType##Register;
}