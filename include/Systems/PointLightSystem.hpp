#ifndef POINTLIGHTSYSTEM_HPP
#define POINTLIGHTSYSTEM_HPP

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

class PointLightSystem {
public:
	PointLightSystem(Device& p_device, VkRenderPass p_renderPass, VkDescriptorSetLayout p_globalSetLayout);
	~PointLightSystem();

	// Delete copy-constructor
	PointLightSystem(const PointLightSystem&) = delete;
	PointLightSystem& operator=(const PointLightSystem&) = delete;

	void update(FrameInfo& p_frameInfo, GlobalUniformBufferObject& p_uniformBufferObject);
	void render(FrameInfo& p_frameInfo);
private:
	Device& m_device;

	VkPipelineLayout m_pipelineLayout;
	std::unique_ptr<Pipeline> m_pipeline;

	void createPipelineLayout(VkDescriptorSetLayout p_globalSetLayout);
	void createPipeline(VkRenderPass p_renderPass);
};

} // FFL

#endif // POINTLIGHTSYSTEM_HPP
