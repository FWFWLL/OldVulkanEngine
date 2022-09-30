#ifndef MODEL_HPP
#define MODEL_HPP

#include "Device.hpp"

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace FFL {

class Model {
public:
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};
public:
	Model(Device& p_device, const std::vector<Vertex>& p_vertices);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
private:
	Device& m_device;
	VkDeviceMemory m_vertexBufferMemory;
	VkBuffer m_vertexBuffer;
	uint32_t m_vertexCount;
private:
	void createVertexBuffers(const std::vector<Vertex>& p_vertices);
public:
	void bind(VkCommandBuffer p_commandBuffer);
	void draw(VkCommandBuffer p_commandBuffer);
};

} // FFL

#endif // MODEL_HPP
