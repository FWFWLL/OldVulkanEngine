#include "Systems/PointLightSystem.hpp"
#include "Camera.hpp"
#include "FrameInfo.hpp"
#include "GameObject.hpp"

// Libraries
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

// STD
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace FFL {

PointLightSystem::PointLightSystem(Device& p_device, VkRenderPass p_renderPass, VkDescriptorSetLayout p_globalSetLayout) : m_device{p_device} {
	createPipelineLayout(p_globalSetLayout);
	createPipeline(p_renderPass);
}

PointLightSystem::~PointLightSystem() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout p_globalSetLayout) {
	// VkPushConstantRange pushConstantRange = {};
	// pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	// pushConstantRange.offset = 0;
	// pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {p_globalSetLayout};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if(vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void PointLightSystem::createPipeline(VkRenderPass p_renderPass) {
	assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig = {};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.attributeDescriptions.clear();
	pipelineConfig.bindingDescriptions.clear();
	pipelineConfig.renderPass = p_renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;

	m_pipeline = std::make_unique<Pipeline>(m_device, pipelineConfig, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv");
}

void PointLightSystem::render(FrameInfo& p_frameInfo) {
	m_pipeline->bind(p_frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(p_frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &p_frameInfo.globalDescriptorSet, 0, nullptr);

	vkCmdDraw(p_frameInfo.commandBuffer, 6, 1, 0, 0);
}

} // FFL
