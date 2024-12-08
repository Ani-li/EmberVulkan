#pragma once
#include "../Setup/eb_setup.hpp"
#include "../RenderPasses/eb_renderPassFactory.hpp"
#include "../Pipeline/eb_pipeline.hpp"

#include <memory>
#include <vector>

namespace eb
{
	class EbRenderer
	{
	public:
		EbRenderer(
			EbSetup& ebSetup
		);
		~EbRenderer();

		void render(EbCamera& cam);
	private:
		uint32_t currentImageIndex;

		EbSetup& ebSetup;
		VkCommandBuffer commandBuffer;
		std::list<std::pair<std::shared_ptr<EbRenderPassFactory>, std::unique_ptr<EbPipeline>>> renderContext;

		void beginFrame();
		void endFrame();
		void createCommandBuffers();
		VkResult submitCommandBuffers();
	};
}