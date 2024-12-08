#pragma once
#include "eb_mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
namespace eb
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f ,1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4()
		{
			auto transform = glm::translate(glm::mat4{ 1.f }, translation);
			transform = glm::rotate(transform, rotation.y, { 0.0f, 1.0f, 0.0f });
			transform = glm::rotate(transform, rotation.x, { 1.0f, 0.0f, 0.0f });
			transform = glm::rotate(transform, rotation.z, { 0.0f, 0.0f, 1.0f });
			transform = glm::scale(transform, scale);
			return transform;
		}
	};

	class EbModel
	{
	public:
		EbModel();
		static EbModel createModel();
		void draw(VkCommandBuffer cmd);

		std::shared_ptr<EbMesh> mesh{};
		TransformComponent transform;

	};
}