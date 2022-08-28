#include "Model.hpp"

#include <cstring>

namespace FFL {

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = 0;

	return attributeDescriptions;
}

Model::Model(Device& p_device, const std::vector<Vertex>& p_vertices) : m_device{p_device} {
	createVertexBuffers(p_vertices);
}

Model::~Model() {
	vkDestroyBuffer(m_device.device(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_device.device(), m_vertexBufferMemory, nullptr);
}

void Model::createVertexBuffers(const std::vector<Vertex>& p_vertices) {
	m_vertexCount = static_cast<uint32_t>(p_vertices.size());
	assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
	
	VkDeviceSize bufferSize = sizeof(p_vertices[0]) * m_vertexCount;
	
	m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vertexBuffer, m_vertexBufferMemory);

	void* data;
	vkMapMemory(m_device.device(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, p_vertices.data(), static_cast<size_t>(bufferSize));

	vkUnmapMemory(m_device.device(), m_vertexBufferMemory);
}

void Model::bind(VkCommandBuffer p_commandBuffer) {
	VkBuffer buffers[] = {m_vertexBuffer};
	VkDeviceSize offsets[] = {0};

	vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, buffers, offsets);
}

void Model::draw(VkCommandBuffer p_commandBuffer) {
	vkCmdDraw(p_commandBuffer, m_vertexCount, 1, 0, 0);
}

} // FFL
