#include "eb_renderer.hpp"
#include "../Model/eb_modelCache.hpp"
#include <cassert>

namespace eb
{
	EbRenderer::EbRenderer(
		EbSetup& ebSetup
	) :ebSetup(ebSetup)
	{
		for (const auto& pass : EbRenderPassFactory::getAllRenderPasses(ebSetup))
		{
			pass->Init();
			std::unique_ptr<EbPipeline> pipeline = std::make_unique<EbPipeline>(pass);
			std::pair<std::shared_ptr<EbRenderPassFactory>, std::unique_ptr<EbPipeline>> context(
				pass,
				std::move(pipeline)
			);
			renderContext.push_back(std::move(context));
		}
		createCommandBuffers();
	}

	EbRenderer::~EbRenderer()
	{

	}

	void EbRenderer::render(EbCamera& cam)
	{
		beginFrame();
		for (const auto& [renderPass, renderPipeline] : renderContext)
		{
			renderPass->beginRenderPass(commandBuffer, currentImageIndex, { 0.01f,0.01f,0.01f,1.0f }, { 1.0f, 0 });
			renderPipeline->bind(commandBuffer);
			renderPass->execute(commandBuffer, cam);
			renderPass->endRenderPass(commandBuffer);
		}
		endFrame();
	}

	void EbRenderer::beginFrame()
	{
		vkWaitForFences(ebSetup.Init_device, 1, &ebSetup.Init_inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(ebSetup.Init_device, 1, &ebSetup.Init_inFlightFence);

		vkAcquireNextImageKHR(ebSetup.Init_device, ebSetup.Init_swapChain, UINT64_MAX, ebSetup.Init_imageAvailableSemaphore, VK_NULL_HANDLE, &currentImageIndex);
		vkResetCommandBuffer(commandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}
	}

	void EbRenderer::endFrame()
	{

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
		submitCommandBuffers();
	}

	void EbRenderer::createCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = ebSetup.Init_commandPool;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(ebSetup.Init_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	VkResult EbRenderer::submitCommandBuffers()
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { ebSetup.Init_imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { ebSetup.Init_renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(ebSetup.Init_graphicsQueue, 1, &submitInfo, ebSetup.Init_inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { ebSetup.Init_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &currentImageIndex;

		auto result = vkQueuePresentKHR(ebSetup.Init_presentQueue, &presentInfo);

		return result;
	}
}