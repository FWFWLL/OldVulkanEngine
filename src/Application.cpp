#include "Application.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
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

namespace FFL {

struct GlobalUniformBufferObject {
	glm::mat4 projectionView{1.0f};
	glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
};

Application::Application() {
	loadGameObjects();
}

Application::~Application() {}

void Application::run() {
	Buffer globalUniformBufferObjectBuffer{m_device, sizeof(GlobalUniformBufferObject), SwapChain::MAX_FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_device.properties.limits.minUniformBufferOffsetAlignment};
	globalUniformBufferObjectBuffer.map();

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
			int frameIndex = m_renderer.getFrameIndex();
			FrameInfo frameInfo = {frameIndex, deltaTime, commandBuffer, camera};

			// Update
			GlobalUniformBufferObject uniformBufferObject{};
			uniformBufferObject.projectionView = camera.getProjection() * camera.getView();
			globalUniformBufferObjectBuffer.writeToIndex(&uniformBufferObject, frameIndex);
			globalUniformBufferObjectBuffer.flushIndex(frameIndex);

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
	flatVase.transform.translation = {-0.5f, 0.5f, 2.5f};
	flatVase.transform.scale = {3.0f, 1.5f, 3.0f};

	m_gameObjects.push_back(std::move(flatVase));

	model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");

	GameObject smoothVase = GameObject::createGameObject();
	smoothVase.model = model;
	smoothVase.transform.translation = {0.5f, 0.5f, 2.5f};
	smoothVase.transform.scale = {3.0f, 1.5f, 3.0f};

	m_gameObjects.push_back(std::move(smoothVase));
}

} // FFL
