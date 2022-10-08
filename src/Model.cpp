#include "Model.hpp"
#include "Utils.hpp"

// Libraries
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan_core.h>

// STD
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace std {

template<>
struct hash<FFL::Model::Vertex> {
	size_t operator()(FFL::Model::Vertex const& p_vertex) const {
		size_t seed = 0;

		FFL::hashCombine(seed, p_vertex.position, p_vertex.color, p_vertex.normal, p_vertex.uv);

		return seed;
	}
};

}

namespace FFL {

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	return attributeDescriptions;
}

void Model::Builder::loadModel(const std::string& p_filePath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, p_filePath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for(const tinyobj::shape_t& shape : shapes) {
		for(const tinyobj::index_t& index : shape.mesh.indices) {
			Vertex vertex = {};

			if(index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				size_t colorIndex = 3 * index.vertex_index + 2;
				if(colorIndex < attrib.colors.size()) {
					vertex.color = {
						attrib.colors[colorIndex - 2],
						attrib.colors[colorIndex - 1],
						attrib.colors[colorIndex - 0],
					};
				} else {
					vertex.color = {1.0f, 1.0f, 1.0f}; // Set default color
				}
			}

			if(index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if(index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}
			
			if(uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

Model::Model(Device& p_device, const Model::Builder& p_builder) : m_device{p_device} {
	createVertexBuffers(p_builder.vertices);
	createIndexBuffer(p_builder.indices);
}

Model::~Model() {
	vkDestroyBuffer(m_device.device(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_device.device(), m_vertexBufferMemory, nullptr);

	if(m_hasIndexBuffer) {
		vkDestroyBuffer(m_device.device(), m_indexBuffer, nullptr);
		vkFreeMemory(m_device.device(), m_indexBufferMemory, nullptr);
	}
}

std::unique_ptr<Model> Model::createModelFromFile(Device& p_device, const std::string& p_filePath) {
	Builder builder = {};
	builder.loadModel(p_filePath);

	std::cout << "Vertex Count: " << builder.vertices.size() << '\n';

	return std::make_unique<Model>(p_device, builder);
}

void Model::createVertexBuffers(const std::vector<Vertex>& p_vertices) {
	m_vertexCount = static_cast<uint32_t>(p_vertices.size());
	assert(m_vertexCount >= 3 && "Vertex count must be at least 3");

	VkDeviceSize bufferSize = sizeof(p_vertices[0]) * m_vertexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, p_vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);
	m_device.copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}


void Model::createIndexBuffer(const std::vector<uint32_t>& p_indices) {
	m_indexCount = static_cast<uint32_t>(p_indices.size());
	m_hasIndexBuffer = m_indexCount > 0;

	if(!m_hasIndexBuffer) {
		return;
	}

	VkDeviceSize bufferSize = sizeof(p_indices[0]) * m_indexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, p_indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);
	m_device.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

void Model::bind(VkCommandBuffer p_commandBuffer) {
	VkBuffer buffers[] = {m_vertexBuffer};
	VkDeviceSize offsets[] = {0};

	vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, buffers, offsets);

	if(m_hasIndexBuffer) {
		vkCmdBindIndexBuffer(p_commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void Model::draw(VkCommandBuffer p_commandBuffer) {
	if(m_hasIndexBuffer) {
		vkCmdDrawIndexed(p_commandBuffer, m_indexCount, 1, 0, 0, 0);
	} else {
		vkCmdDraw(p_commandBuffer, m_vertexCount, 1, 0, 0);
	}
}

} // FFL
