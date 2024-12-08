#include "eb_camera.hpp"

#include <cassert>
#include <limits>
#include <iostream>

namespace eb
{
	EbCamera::EbCamera(GLFWwindow* window) : viewControler(window, this)
	{

	}

	void EbCamera::setOrthographicProjection(
		float left, float right, float top, float bottom, float near, float far) {
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = 2.f / (right - left);
		projectionMatrix[1][1] = 2.f / (bottom - top);
		projectionMatrix[2][2] = 1.f / (far - near);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -near / (far - near);
	}

	void EbCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = far / (far - near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(far * near) / (far - near);
	}

	void EbCamera::updateViewMatrix()
	{
		viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = direct.right.x;
		viewMatrix[1][0] = direct.right.y;
		viewMatrix[2][0] = direct.right.z;
		viewMatrix[0][1] = -direct.up.x;
		viewMatrix[1][1] = -direct.up.y;
		viewMatrix[2][1] = -direct.up.z;
		viewMatrix[0][2] = direct.forward.x;
		viewMatrix[1][2] = direct.forward.y;
		viewMatrix[2][2] = direct.forward.z;
		viewMatrix[3][0] = -glm::dot(direct.right, position);
		viewMatrix[3][1] = -glm::dot(-direct.up, position);
		viewMatrix[3][2] = -glm::dot(direct.forward, position);
	}

	void EbCamera::setPosition(
		glm::vec3 pos
	)
	{
		position = pos;
		updateViewMatrix();
	}

	void EbCamera::setForwardDir(
		glm::vec3 forward
	)
	{
		direct.forward = glm::normalize(forward);

		glm::vec3 worldUp(0.0f, -1.0f, 0.0f);
		direct.right = glm::normalize(glm::cross(direct.forward, worldUp));
		direct.up = glm::normalize(glm::cross(direct.right, direct.forward));
		updateViewMatrix();
	}

	void EbCamera::setTarget(
		glm::vec3 pos
	)
	{
		target = pos;
		setForwardDir(target - position);
	}

	void EbCamera::screenInput_rotateAroundTarget(
		float angleDegreesX,
		float angleDegreesY
	)
	{
		float angleRadians_x = glm::radians(angleDegreesX);
		float angleRadians_y = glm::radians(angleDegreesY);
		glm::vec3 worldUp(0.0f, -1.0f, 0.0f);

		glm::vec3 relativePos = position - target;
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians_x, worldUp);
		glm::vec4 rotatedRelative = rotationMatrix * glm::vec4(relativePos, 1.0f);
		position = glm::vec3(rotatedRelative) + target;

		direct.forward = glm::normalize(target - position);
		direct.right = glm::normalize(glm::cross(direct.forward, worldUp));
		direct.up = glm::normalize(glm::cross(direct.right, direct.forward));

		relativePos = position - target;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians_y, direct.right);
		rotatedRelative = rotationMatrix * glm::vec4(relativePos, 1.0f);
		position = glm::vec3(rotatedRelative) + target;

		direct.forward = glm::normalize(target - position);
		direct.right = glm::normalize(glm::cross(direct.forward, worldUp));
		direct.up = glm::normalize(glm::cross(direct.right, direct.forward));

		if (glm::dot(worldUp, direct.up) < 0.01f) {
			return;
		}

		updateViewMatrix();
	}

	void EbCamera::screenInput_moveTarget(
		float X,
		float Y
	)
	{
		glm::vec3 curTarget = target;
		curTarget = target + X * direct.right;
		curTarget = curTarget + Y * direct.up;

		setTarget(curTarget);
	}

	void EbCamera::screenInput_zoomTarget(
		float Y
	)
	{
		glm::vec3 curPosition = position;
		curPosition = position + Y * direct.forward;

		setPosition(curPosition);
	}
}