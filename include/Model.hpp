#ifndef MODEL_HPP
#define MODEL_HPP

#include "Device.hpp"

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// STD
#include <cstdint>
#include <vector>

namespace FFL {

class Model {
public:
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	struct Data {
		std::vector<Vertex> vertices = {};
		std::vector<uint32_t> indices = {};
	};

	Model(Device& p_device, const Model::Data& p_builder);
	~Model();

	// Delete copy-constructors
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer p_commandBuffer);
	void draw(VkCommandBuffer p_commandBuffer);
private:
	Device& m_device;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
	uint32_t m_vertexCount;

	bool m_hasIndexBuffer = false;
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;
	uint32_t m_indexCount;

	void createVertexBuffers(const std::vector<Vertex>& p_vertices);
	void createIndexBuffer(const std::vector<uint32_t>& p_indices);
};

} // FFL

#endif // MODEL_HPP
