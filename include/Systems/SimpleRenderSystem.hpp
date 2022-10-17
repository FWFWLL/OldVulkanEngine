#ifndef SIMPLERENDERSYSTEM_HPP
#define SIMPLERENDERSYSTEM_HPP

#include "Camera.hpp"
#include "Device.hpp"
#include "FrameInfo.hpp"
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
	SimpleRenderSystem(Device& p_device, VkRenderPass p_renderPass, VkDescriptorSetLayout p_globalSetLayout);
	~SimpleRenderSystem();

	// Delete copy-constructor
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(FrameInfo& p_frameInfo);
private:
	Device& m_device;

	VkPipelineLayout m_pipelineLayout;
	std::unique_ptr<Pipeline> m_pipeline;

	void createPipelineLayout(VkDescriptorSetLayout p_globalSetLayout);
	void createPipeline(VkRenderPass p_renderPass);
};

} // FFL

#endif // SIMPLERENDERSYSTEM_HPP
