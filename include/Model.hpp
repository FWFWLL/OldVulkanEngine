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

		Model(Device& p_device, const std::vector<Vertex>& p_vertices);
		~Model();

		// Delete copy-constructors
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer p_commandBuffer);
		void draw(VkCommandBuffer p_commandBuffer);
	private:
		Device& m_device;
		VkDeviceMemory m_vertexBufferMemory;
		VkBuffer m_vertexBuffer;
		uint32_t m_vertexCount;

		void createVertexBuffers(const std::vector<Vertex>& p_vertices);
};

} // FFL

#endif // MODEL_HPP
