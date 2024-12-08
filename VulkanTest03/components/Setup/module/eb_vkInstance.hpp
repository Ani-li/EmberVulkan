#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>
#include "eb_validationLayers.hpp"



namespace eb {
	class EbSetup;

	class EbVkInstance
	{
	private:
		EbSetup* SetupManager;

	public:
		EbVkInstance(EbSetup* manager);
		~EbVkInstance();
		EbValidationLayers ebValidationLayers;

	private:
		void createInstance();
		VkInstance instance;
		std::vector<const char*> getRequireLayers();
		std::vector<const char*> getRequireExtensions();

	};
}