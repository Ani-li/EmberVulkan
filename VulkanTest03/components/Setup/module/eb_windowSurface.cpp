#include "../eb_setup.hpp"
#include "eb_windowSurface.hpp"


namespace eb {
	EbWindowSurface::EbWindowSurface(VkInstance instance, GLFWwindow* window, EbSetup* manager) : vkInstance(instance), window(window), SetupManager(manager)
	{
		createSurface();
	}

	EbWindowSurface::~EbWindowSurface() {
		vkDestroySurfaceKHR(vkInstance, surface, nullptr);
	}

	void EbWindowSurface::createSurface() {
		if (glfwCreateWindowSurface(vkInstance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
		if (SetupManager) {
			SetupManager->Init_surface = surface;
		}
	}
}