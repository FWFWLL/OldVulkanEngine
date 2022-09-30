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

void Application::loadGameObjects() {
	std::vector<Model::Vertex> vertices {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	};

	auto model = std::make_shared<Model>(m_device, vertices);

	auto triangle = GameObject::createGameObject();
	triangle.model = model;
	triangle.color = {0.1f, 0.8f, 0.1f};
	triangle.transform2D.translation.x = 0.2f;
	triangle.transform2D.scale = {2.0f, 0.5f};
	triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

	m_gameObjects.push_back(std::move(triangle));
}

} // FFL
