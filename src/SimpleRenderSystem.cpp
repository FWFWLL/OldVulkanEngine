#include "SimpleRenderSystem.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>
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

struct SimplePushConstantData {
	glm::mat4 transform{1.0f};
	glm::mat4 normalMatrix{1.0f};
};

SimpleRenderSystem::SimpleRenderSystem(Device& p_device, VkRenderPass p_renderPass) : m_device{p_device} {
	createPipelineLayout();
	createPipeline(p_renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout() {
	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if(vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void SimpleRenderSystem::createPipeline(VkRenderPass p_renderPass) {
	assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig = {};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = p_renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;

	m_pipeline = std::make_unique<Pipeline>(m_device, pipelineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer p_commandBuffer, std::vector<GameObject>& p_gameObjects, const Camera& p_camera) {
	m_pipeline->bind(p_commandBuffer);

	glm::mat4 projectionView = p_camera.getProjection() * p_camera.getView();

	for(GameObject& obj : p_gameObjects) {
		glm::mat4 normalMatrix = obj.transform.mat4();

		SimplePushConstantData push = {};
		push.transform = projectionView * normalMatrix;
		push.normalMatrix = normalMatrix;

		vkCmdPushConstants(p_commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

		obj.model->bind(p_commandBuffer);
		obj.model->draw(p_commandBuffer);
	}
}

} // FFL
