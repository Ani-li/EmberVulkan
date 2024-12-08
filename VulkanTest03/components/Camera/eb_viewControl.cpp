#include "eb_viewControl.hpp"
#include "eb_camera.hpp"

namespace eb
{
	double EbViewControl::zoom = 0.0f;

	EbViewControl::EbViewControl(GLFWwindow* window, EbCamera* camera) : window(window), camera(camera)
	{
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			EbViewControl::zoom = yoffset;
			});
	}

	void EbViewControl::controlViewingViaMouse()
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (!isLeftDragging) {
				isLeftDragging = true;
				glfwGetCursorPos(window, &lastX, &lastY);
			}

			double currentX, currentY;
			glfwGetCursorPos(window, &currentX, &currentY);

			double deltaX = currentX - lastX;
			double deltaY = currentY - lastY;

			camera->screenInput_rotateAroundTarget(
				static_cast<float>(-deltaX * lookSpeed),
				static_cast<float>(-deltaY * lookSpeed)
			);

			lastX = currentX;
			lastY = currentY;
		}
		else {
			isLeftDragging = false;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			if (!isMiddleDragging) {
				isMiddleDragging = true;
				glfwGetCursorPos(window, &lastX, &lastY);
			}

			double currentX, currentY;
			glfwGetCursorPos(window, &currentX, &currentY);

			double deltaX = currentX - lastX;
			double deltaY = currentY - lastY;

			camera->screenInput_moveTarget(
				static_cast<float>(-deltaX * moveSpeed),
				static_cast<float>(deltaY * moveSpeed)
			);

			lastX = currentX;
			lastY = currentY;
		}
		else {
			isMiddleDragging = false;
		}

		camera->screenInput_zoomTarget(static_cast<float>(zoom * zoomSpeed));
		zoom = 0;
	}
}