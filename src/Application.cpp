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

	GameObject viewerObject = GameObject::createGameObject();
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

void Application::loadGameObjects() {
	std::shared_ptr<Model> model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");

	GameObject gameObj = GameObject::createGameObject();
	gameObj.model = model;
	gameObj.transform.translation = {0.0f, 0.0f, 2.5f};
	gameObj.transform.scale = {0.5f, 0.5f, 0.5f};

	m_gameObjects.push_back(std::move(gameObj));
}

} // FFL
