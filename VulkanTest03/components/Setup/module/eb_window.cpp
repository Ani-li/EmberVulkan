#include "eb_window.hpp"
#include "../eb_setup.hpp"
#include "../../Utils/eb_utils.hpp"

namespace eb {
	EbWindow::EbWindow(int w, int h, std::string name, EbSetup* manager) : width(w), height(h), windowName(name), SetupManager(manager)
	{
		initWindow();
	}

	EbWindow::~EbWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void EbWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		if (SetupManager) {
			SetupManager->Init_window = window;
			SetupManager->Init_windowAspectRatio = EbUtils::CalAspectRatio(width, height);
		}
	}
}