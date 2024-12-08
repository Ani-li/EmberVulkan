#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include "../../Utils/eb_utils.hpp"


namespace eb {
	class EbSetup;

	class EbSwapChain
	{
	private:
		EbSetup* SetupManager;

	public:
		EbSwapChain(
			VkDevice device,
			VkPhysicalDevice physicalDevice,
			VkSurfaceKHR surface,
			GLFWwindow* window,
			EbSetup* manager
		);
		~EbSwapChain();

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	private:
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		uint32_t swapChainImageCount;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkDevice device;
		void createSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window);
		void createSwapChainImageViews();
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		VkSurfaceFormatKHR chooseSwapSufaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
	};
}