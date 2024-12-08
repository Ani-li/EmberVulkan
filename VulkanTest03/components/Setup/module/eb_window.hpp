#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace eb {
	class EbSetup;

	class EbWindow
	{
	private:
		EbSetup* SetupManager;

	public:
		EbWindow(int w, int h, std::string name, EbSetup* manager);
		~EbWindow();

		GLFWwindow* window;

		//EbWindow(const EbWindow&) = delete;// ���ÿ������캯��
		//EbWindow& operator=(const EbWindow&) = delete;// ���ÿ�����ֵ�����
		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;

	};
}