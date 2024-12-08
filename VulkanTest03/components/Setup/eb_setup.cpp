#include "eb_setup.hpp"

namespace eb {
	EbSetup::EbSetup(
		int window_width,
		int window_height,
		std::string window_name
	) :
		ebWindow(window_width, window_height, window_name, this),
		ebVkInstance(this),
		ebWindowSurface(Init_vkInstance, Init_window, this),
		ebLogicalDevicesAndQueue(Init_vkInstance, Init_surface, ebVkInstance.ebValidationLayers.getValidationLayersCount(), ebVkInstance.ebValidationLayers.getValidationLayers(), this),
		ebCommandPool(Init_device, Init_graphicsFamily, Init_graphicsQueue, this),
		ebSwapChain(Init_device, Init_physicalDevice, Init_surface, Init_window, this),
		ebSyncObject(Init_device, this)
	{

	}

	EbSetup::~EbSetup()
	{

	}

	bool EbSetup::windowShouldClose() {
		return ebWindow.shouldClose();
	}


}