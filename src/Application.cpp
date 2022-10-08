#include "Application.hpp"
#include "Camera.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "KeyboardMovementController.hpp"
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

namespace FFL {

Application::Application() {
	loadGameObjects();
}

Application::~Application() {}

void Application::run() {
	SimpleRenderSystem simpleRenderSystem{m_device, m_renderer.getSwapchainRenderPass()};
	Camera camera{};

	auto viewerObject = GameObject::createGameObject();
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
		camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

		if(VkCommandBuffer commandBuffer = m_renderer.beginFrame()) {
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, m_gameObjects, camera);
			m_renderer.endSwapChainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(m_device.device());
}

// Temporary helper function, creates a 1^3 cube centered at offset
std::unique_ptr<Model> createCubeModel(Device& p_device, glm::vec3 p_offset) {
	Model::Data modelData = {};

	modelData.vertices = {
		// Left face (white)
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, 0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, -0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
		{{-0.5f, 0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},

		// Right face (yellow)
		{{0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, -0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},

		// Top face (orange, remember y axis points down)
		{{-0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
		{{0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
		{{-0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
		{{0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},

		// Bottom face (red)
		{{-0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
		{{0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
		{{-0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
		{{0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},

		// Nose face (blue)
		{{-0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
		{{0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
		{{-0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
		{{0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},

		// Tail face (green)
		{{-0.5f, -0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
		{{0.5f, 0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
		{{-0.5f, 0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
		{{0.5f, -0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
	};

	for(auto& v : modelData.vertices) {
		v.position += p_offset;
	}

	modelData.indices = {
		// Left face
		0, 1, 2,
		0, 3, 1,

		// Right face
		4, 5, 6,
		4, 7, 5,

		// Top face
		8, 9, 10,
		8, 11, 9,

		// Bottom face
		12, 13, 14,
		12, 15, 13,

		// Nose face
		16, 17, 18,
		16, 19, 17,

		// Tail face
		20, 21, 22,
		20, 23, 21,
	};

	return std::make_unique<Model>(p_device, modelData);
}

void Application::loadGameObjects() {
	std::shared_ptr<Model> model = createCubeModel(m_device, {0.0f, 0.0f, 0.0f});

	GameObject cube = GameObject::createGameObject();
	cube.model = model;
	cube.transform.translation = {0.0f, 0.0f, 2.5f};
	cube.transform.scale = {0.5f, 0.5f, 0.5f};

	m_gameObjects.push_back(std::move(cube));
}

} // FFL
