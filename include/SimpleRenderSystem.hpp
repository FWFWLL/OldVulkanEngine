#ifndef SIMPLERENDERSYSTEM_HPP
#define SIMPLERENDERSYSTEM_HPP

#include "Device.hpp"
#include "GameObject.hpp"
#include "Pipeline.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <memory>
#include <vector>

namespace FFL {

class SimpleRenderSystem {
	public:
		SimpleRenderSystem(Device& p_device, VkRenderPass p_renderPass);
		~SimpleRenderSystem();

		// Delete copy-constructor
		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer p_commandBuffer, std::vector<GameObject>& p_gameObjects);
	private:
		Device& m_device;

		VkPipelineLayout m_pipelineLayout;
		std::unique_ptr<Pipeline> m_pipeline;

		void createPipelineLayout();
		void createPipeline(VkRenderPass p_renderPass);
};

}

#endif // SIMPLERENDERSYSTEM_HPP
