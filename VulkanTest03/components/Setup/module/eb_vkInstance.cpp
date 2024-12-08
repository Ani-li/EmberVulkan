#include "eb_vkInstance.hpp"
#include "../eb_setup.hpp"

namespace eb {
	EbVkInstance::EbVkInstance(EbSetup* manager) : SetupManager(manager)
	{
		createInstance();
		ebValidationLayers.setupDebugMessenger(instance);
	}

	EbVkInstance::~EbVkInstance() {
		ebValidationLayers.DestroyDebugUtilsMessengerEXT(instance);
		vkDestroyInstance(instance, nullptr);
	}

	void EbVkInstance::createInstance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "vulkan test02";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequireExtensions();
		auto layers = getRequireLayers();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = layers.data();

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		if (SetupManager) {
			SetupManager->Init_vkInstance = instance;
		}
	}

	std::vector<const char*> EbVkInstance::getRequireExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions;

		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		if (ebValidationLayers.getValidationExtension() != nullptr) {
			extensions.push_back(ebValidationLayers.getValidationExtension());
		}

		const char* extensionNames[] = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME, // ÆôÓÃµ÷ÊÔÀ©Õ¹
		};

		extensions.push_back(extensionNames[0]);

		return extensions;
	}
	std::vector<const char*> EbVkInstance::getRequireLayers() {
		std::vector<const char*> layers;
		for (uint32_t i = 0; i < ebValidationLayers.getValidationLayersCount(); i++) {
			layers.push_back(ebValidationLayers.getValidationLayers()[i]);
		}
		return layers;
	}
}