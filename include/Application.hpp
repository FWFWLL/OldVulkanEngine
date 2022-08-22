#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "Pipeline.hpp"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

namespace FFL {

class Application {
public:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
private:
	Window m_window = {SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan_C++"};
	Device m_device = {m_window};
	SwapChain m_swapChain = {m_device, m_window.getExtent()};
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
public:
	static const uint32_t SCREEN_WIDTH = 640;
	static const uint32_t SCREEN_HEIGHT = 640;
private:
	void createPipelineLayout();
	void createPipeline();
	void createCommandBufffers();
	void drawFrame();
public:
	void run();
};

}

#endif // APPLICATION_HPP
