#include "Application.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Descriptors.hpp"
#include "Device.hpp"
#include "FrameInfo.hpp"
#include "GameObject.hpp"
#include "KeyboardMovementController.hpp"
#include "Pipeline.hpp"
#include "SimpleRenderSystem.hpp"
#include "SwapChain.hpp"

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
#include <vulkan/vulkan_core.h>

// STD
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace FFL {

struct GlobalUniformBufferObject {
	glm::mat4 projectionView{1.0f};
	glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // W is intensity
	glm::vec3 lightPosition{-1.0f}; // Ignore W
	alignas(16) glm::vec4 lightColor{1.0f}; // W is light intensity
};

Application::Application() {
	m_globalPool = DescriptorPool::Builder(m_device)
		.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();

	loadGameObjects();
}

Application::~Application() {}

void Application::run() {
	std::vector<std::unique_ptr<Buffer>> uniformBufferObjectBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(std::unique_ptr<Buffer>& ubo : uniformBufferObjectBuffers) {
		ubo = std::make_unique<Buffer>(m_device, sizeof(GlobalUniformBufferObject), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_device.properties.limits.minUniformBufferOffsetAlignment);
		ubo->map();
	}

	std::unique_ptr<DescriptorSetLayout> globalSetLayout = DescriptorSetLayout::Builder(m_device)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.build();

	std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(size_t i = 0; i < globalDescriptorSets.size(); i++) {
		VkDescriptorBufferInfo bufferInfo = uniformBufferObjectBuffers[i]->descriptorInfo();
		DescriptorWriter(*globalSetLayout, *m_globalPool)
			.writeBuffer(0, &bufferInfo)
			.build(globalDescriptorSets[i]);
	}

	SimpleRenderSystem simpleRenderSystem{m_device, m_renderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
	Camera camera{};

	GameObject viewerObject = GameObject::createGameObject();
	viewerObject.transform.translation.z = -2.5f;
	KeyboardMovementController cameraController{};

	auto currentTime = std::chrono::high_resolution_clock::now();

	while(!m_window.shouldClose()) {
		glfwPollEvents();

		auto newTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		deltaTime = glm::min(deltaTime, 1.0f);

		cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), deltaTime, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

		float aspect = m_renderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

		if(VkCommandBuffer commandBuffer = m_renderer.beginFrame()) {
			int frameIndex = m_renderer.getFrameIndex();
			FrameInfo frameInfo = {frameIndex, deltaTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

			// Update
			GlobalUniformBufferObject uniformBufferObject{};
			uniformBufferObject.projectionView = camera.getProjection() * camera.getView();
			uniformBufferObjectBuffers[frameIndex]->writeToBuffer(&uniformBufferObject);
			uniformBufferObjectBuffers[frameIndex]->flush();

			// Render
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(frameInfo, m_gameObjects);
			m_renderer.endSwapChainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(m_device.device());
}

void Application::loadGameObjects() {
	std::shared_ptr<Model> model = Model::createModelFromFile(m_device, "models/flat_vase.obj");

	GameObject flatVase = GameObject::createGameObject();
	flatVase.model = model;
	flatVase.transform.translation = {-0.5f, 0.5f, 0.0f};
	flatVase.transform.scale = {3.0f, 1.5f, 3.0f};

	m_gameObjects.push_back(std::move(flatVase));

	model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");

	GameObject smoothVase = GameObject::createGameObject();
	smoothVase.model = model;
	smoothVase.transform.translation = {0.5f, 0.5f, 0.0f};
	smoothVase.transform.scale = {3.0f, 1.5f, 3.0f};

	m_gameObjects.push_back(std::move(smoothVase));

	model = Model::createModelFromFile(m_device, "models/quad.obj");

	GameObject floor = GameObject::createGameObject();
	floor.model = model;
	floor.transform.translation = {0.0f, 0.5f, 0.0f};
	floor.transform.scale = {3.0f, 1.0f, 3.0f};

	m_gameObjects.push_back(std::move(floor));
}

} // FFL
