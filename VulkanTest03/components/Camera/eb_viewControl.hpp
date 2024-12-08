#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

namespace eb
{
	class EbCamera;
	class EbViewControl
	{
	public:
		EbViewControl(GLFWwindow* window, EbCamera* camera);
		void controlViewingViaMouse();

		static double zoom;
		double lastX, lastY;
		bool isLeftDragging = false;
		bool isMiddleDragging = false;
		float moveSpeed = 0.005f;
		float lookSpeed = 0.6f;
		float zoomSpeed = 0.1f;

	private:
		GLFWwindow* window;
		EbCamera* camera;
	};
}