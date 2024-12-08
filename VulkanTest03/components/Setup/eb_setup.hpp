#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstring>

#include "module/eb_vkInstance.hpp"
#include "module/eb_window.hpp"
#include "module/eb_windowSurface.hpp"
#include "module/eb_logicalDevicesAndQueue.hpp"
#include "module/eb_commandPool.hpp"
#include "module/eb_swapChain.hpp"
#include "module/eb_syncObject.hpp"

namespace eb {
	class EbSetup
	{
	public:
		VkInstance Init_vkInstance;
		GLFWwindow* Init_window;
		VkSurfaceKHR Init_surface;
		VkDevice Init_device;
		VkPhysicalDevice Init_physicalDevice = VK_NULL_HANDLE;
		std::optional<uint32_t> Init_graphicsFamily;
		std::optional<uint32_t> Init_presentFamily;
		VkQueue Init_graphicsQueue;
		VkQueue Init_presentQueue;
		VkCommandPool Init_commandPool;
		float Init_windowAspectRatio;
		VkSwapchainKHR Init_swapChain;
		VkFormat Init_swapChainImageFormat;
		VkExtent2D Init_swapChainExtent;
		std::vector<VkImageView> Init_swapChainImageViews;
		uint32_t Init_swapChainImageCount;
		VkSemaphore Init_imageAvailableSemaphore;
		VkSemaphore Init_renderFinishedSemaphore;
		VkFence Init_inFlightFence;

	public:
		EbSetup(
			int window_width,
			int window_height,
			std::string window_name
		);
		~EbSetup();

		bool windowShouldClose();

	private:
		/// <summary>
		/// The order of members affects the order of constructor and destructor execution
		/// </summary>
		EbWindow ebWindow;
		EbVkInstance ebVkInstance;
		EbWindowSurface ebWindowSurface;
		EbLogicalDevicesAndQueue ebLogicalDevicesAndQueue;
		EbCommandPool ebCommandPool;
		EbSwapChain ebSwapChain;
		EbSyncObject ebSyncObject;
	};
}

