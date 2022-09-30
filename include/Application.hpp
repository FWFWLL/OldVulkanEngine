#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "Pipeline.hpp"
#include "GameObject.hpp"

// STD
#include <memory>

namespace FFL {

class Application {
public:
	Application();
	~Application();

	// Delete copy-constructor
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
private:
	Window m_window = {SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan_C++"};
	Device m_device = {m_window};
	std::unique_ptr<SwapChain> m_swapChain;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector<GameObject> m_gameObjects;
public:
	static const uint32_t SCREEN_WIDTH = 640;
	static const uint32_t SCREEN_HEIGHT = 640;
private:
	void loadGameObjects();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void freeCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int p_imageIndex);
	void renderGameObjects(VkCommandBuffer p_commandBuffer);
public:
	void run();
};

}

#endif // APPLICATION_HPP
