#pragma	once
#include <string>
#include <vector>
#include "../RenderPasses/eb_renderPassFactory.hpp"

namespace eb
{
	class EbPipeline
	{
	public:
		EbPipeline(
			std::shared_ptr<EbRenderPassFactory> baseRenderPass
		);
		~EbPipeline();

		void bind(VkCommandBuffer commandBuffer);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(
			const PipelineConfigInfo& configInfo
		);

		void createShaderModule(std::vector<char>& code, VkShaderModule& shaderModule);

		EbSetup& ebSetup;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}