#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "eb_viewControl.hpp"

namespace eb
{
	class EbCamera
	{
	public:
		EbCamera(GLFWwindow* window);

		struct Direction
		{
			glm::vec3 up = { 0.0f, -1.0f, 0.0f };
			glm::vec3 forward = { 0.0f, 0.0f, -1.0f };
			glm::vec3 right = { 1.0f, 0.0f, 0.0f };
		};

		void setOrthographicProjection(
			float left,
			float right,
			float top,
			float bottom,
			float near,
			float far
		);

		void setPerspectiveProjection(
			float fovy,
			float aspect,
			float near,
			float far
		);

		void setPosition(
			glm::vec3 pos
		);

		void setForwardDir(
			glm::vec3 forward
		);

		void setTarget(
			glm::vec3 target
		);

		void screenInput_rotateAroundTarget(
			float angleDegreesX,
			float angleDegreesY
		);

		void screenInput_moveTarget(
			float X,
			float Y
		);

		void screenInput_zoomTarget(
			float Y
		);

		const glm::mat4& getProjection() const { return projectionMatrix; }
		const glm::mat4& getView() const { return viewMatrix; }
		void controlView() { return viewControler.controlViewingViaMouse(); }

	private:
		void updateViewMatrix();

		glm::mat4 projectionMatrix{ 1.0f };
		glm::mat4 viewMatrix{ 1.0f };

		glm::vec3 position{ 0.0f,0.0f,0.0f };
		glm::vec3 target{ 0.0f,0.0f,0.0f };
		Direction direct;

		EbViewControl viewControler;
	};

}