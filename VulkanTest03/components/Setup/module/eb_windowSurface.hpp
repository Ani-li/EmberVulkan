#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>

namespace eb {
	class EbSetup;

	class EbWindowSurface
	{
	private:
		EbSetup* SetupManager;

	public:
		EbWindowSurface(VkInstance instance, GLFWwindow* window, EbSetup* manager);
		~EbWindowSurface();

	private:
		VkInstance vkInstance;
		GLFWwindow* window;
		VkSurfaceKHR surface;

		void createSurface();

	};
}