#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>

namespace eb {
	class EbValidationLayers
	{
	public:
		EbValidationLayers();

		uint32_t getValidationLayersCount();
		const char* const* getValidationLayers();
		const char* getValidationExtension();
		void setupDebugMessenger(VkInstance instance);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance);

	private:
		const std::vector<const char*> validationLayerName = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif // NDEBUG

		VkDebugUtilsMessengerEXT debugMessenger;

		bool checkValidationLayerSupport();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);
		static std::string messageSeverityToString(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
		static std::string typeToString(VkDebugUtilsMessageTypeFlagsEXT messageType);

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	};
}