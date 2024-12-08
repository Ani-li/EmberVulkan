#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "../Utils/eb_utils.hpp"
#include "eb_mesh.hpp"
#include <cassert>
#include <unordered_map>

namespace eb
{
	EbMesh::EbMesh(
		EbSetup& ebSetup,
		const EbMesh::Builder& builder
	) :ebSetup(ebSetup)
	{
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
	}

	EbMesh::~EbMesh()
	{
		vkDestroyBuffer(ebSetup.Init_device, vertexBuffer, nullptr);
		vkFreeMemory(ebSetup.Init_device, vertexBufferMemory, nullptr);

		vkDestroyBuffer(ebSetup.Init_device, indexBuffer, nullptr);
		vkFreeMemory(ebSetup.Init_device, indexBufferMemory, nullptr);
	}

	void EbMesh::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be as least 3!");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		EbUtils::createBuffer(
			ebSetup.Init_device,
			ebSetup.Init_physicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory
		);
		void* data;
		vkMapMemory(ebSetup.Init_device, vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(ebSetup.Init_device, vertexBufferMemory);
	}

	void EbMesh::createIndexBuffers(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		EbUtils::createBuffer(
			ebSetup.Init_device,
			ebSetup.Init_physicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexBuffer,
			indexBufferMemory
		);
		void* data;
		vkMapMemory(ebSetup.Init_device, indexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(ebSetup.Init_device, indexBufferMemory);
	}

	void EbMesh::draw(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

	std::unique_ptr<EbMesh> EbMesh::createModelFromFile(
		EbSetup& ebSetup,
		const std::string& filepath
	)
	{
		Builder builder{};
		builder.loadModel(filepath);
		return std::make_unique<EbMesh>(ebSetup, builder);
	}

	std::unique_ptr<EbMesh> EbMesh::createFullScreenModel(
		EbSetup& ebSetup
	)
	{
		Builder builder{};
		builder.vertices = {
			{{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // 左下
			{{ 1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 右下
			{{ 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 右上
			{{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // 左上
		};
		builder.indices = {
			0, 1, 2, // 第一个三角形
			2, 3, 0  // 第二个三角形
		};
		return std::make_unique<EbMesh>(ebSetup, builder);
	}

	std::vector<VkVertexInputBindingDescription> EbMesh::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> EbMesh::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		attributeDescriptions.push_back({ 0,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2,0,VK_FORMAT_R32G32B32_SFLOAT,offsetof(Vertex, normal) });
		attributeDescriptions.push_back({ 3,0,VK_FORMAT_R32G32_SFLOAT,offsetof(Vertex, uv) });
		return attributeDescriptions;
	}

	void EbMesh::Builder::loadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<uint32_t, uint32_t>uniqueVertices{};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				if (index.vertex_index >= 0)
				{
					vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2]
					};
				}

				if (index.normal_index >= 0)
				{
					vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (index.texcoord_index >= 0)
				{
					vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (uniqueVertices.count(index.vertex_index) == 0) {
					uniqueVertices[index.vertex_index] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[index.vertex_index]);
			}
		}
	}
}