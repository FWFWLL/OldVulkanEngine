#ifndef MODEL_HPP
#define MODEL_HPP

#include "Device.hpp"
#include "Buffer.hpp"

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// STD
#include <cstdint>
#include <memory>
#include <vector>

namespace FFL {

class Model {
public:
	struct Vertex {
		glm::vec3 position = {};
		glm::vec3 color = {};
		glm::vec3 normal = {};
		glm::vec2 uv = {};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& p_other) const {
			return position == p_other.position && color == p_other.color && normal == p_other.normal && uv == p_other.uv;
		}
	};

	struct Builder {
		std::vector<Vertex> vertices = {};
		std::vector<uint32_t> indices = {};

		void loadModel(const std::string& p_filePath);
	};

	Model(Device& p_device, const Model::Builder& p_builder);
	~Model();

	// Delete copy-constructors
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	static std::unique_ptr<Model> createModelFromFile(Device& p_device, const std::string& p_filePath);

	void bind(VkCommandBuffer p_commandBuffer);
	void draw(VkCommandBuffer p_commandBuffer);
private:
	Device& m_device;

	std::unique_ptr<Buffer> m_vertexBuffer;
	uint32_t m_vertexCount;

	bool m_hasIndexBuffer = false;
	std::unique_ptr<Buffer> m_indexBuffer;
	uint32_t m_indexCount;

	void createVertexBuffers(const std::vector<Vertex>& p_vertices);
	void createIndexBuffer(const std::vector<uint32_t>& p_indices);
};

} // FFL

#endif // MODEL_HPP
