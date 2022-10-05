#include "Application.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Pipeline.hpp"
#include "SimpleRenderSystem.hpp"
#include "SwapChain.hpp"

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

namespace FFL {

Application::Application() {
	loadGameObjects();
}

Application::~Application() {}

void Application::run() {
	SimpleRenderSystem simpleRenderSystem{m_device, m_renderer.getSwapchainRenderPass()};

	while(!m_window.shouldClose()) {
		glfwPollEvents();

		if(VkCommandBuffer commandBuffer = m_renderer.beginFrame()) {
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, m_gameObjects);
			m_renderer.endSwapChainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(m_device.device());
}

// Temporary helper function, creates a 1^3 cube centered at offset
std::unique_ptr<Model> createCubeModel(Device& p_device, glm::vec3 p_offset) {
	std::vector<Model::Vertex> vertices{
		// left face (white)
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, 0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, -0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, 0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, 0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},

		// right face (yellow)
		{{0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, -0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},

		// top face (orange, remember y axis points down)
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
		{{0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
		{{-0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
		{{0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
		{{0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},

		// bottom face (red)
		{{-0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
		{{-0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
		{{-0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
		{{0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},

		// nose face (blue)
		{{-0.5f, -0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},
		{{0.5f, 0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},
		{{-0.5f, 0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},
		{{-0.5f, -0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},
		{{0.5f, -0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},
		{{0.5f, 0.5f, 00.5f}, {0.1f, 0.1f, 0.8f}},

		// tail face (green)
		{{-0.5f, -0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
		{{-0.5f, 0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
		{{-0.5f, -0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
		{{0.5f, -0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, -00.5f}, {0.1f, 0.8f, 0.1f}},
	};

	for(auto& v : vertices) {
		v.position += p_offset;
	}

	return std::make_unique<Model>(p_device, vertices);
}

void Application::loadGameObjects() {
	std::shared_ptr<Model> model = createCubeModel(m_device, {0.0f, 0.0f, 0.0f});

	GameObject cube = GameObject::createGameObject();
	cube.model = model;
	cube.transform.translation = {0.0f, 0.0f, 0.5f};
	cube.transform.scale = {0.5f, 0.5f, 0.5f};

	m_gameObjects.push_back(std::move(cube));
}

} // FFL
