#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

// Libraries
#include <cstdint>
#include <vulkan/vulkan_core.h>

// STD
#include <cassert>
#include <memory>
#include <vector>

namespace FFL {

class Renderer {
public:
	Renderer(Window& p_window, Device& p_device);
	~Renderer();

	// Delete copy-constructor
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	VkRenderPass getSwapchainRenderPass() const {return m_swapChain->getRenderPass();}
	bool isFrameInProgress() const {return m_isFrameStarted;}

	VkCommandBuffer getCurrentCommandBuffer() const {
		assert(m_isFrameStarted && "Cannot get command buffer when frame is not in progress");
		return m_commandBuffers[m_currentFrameIndex];
	}

	int getFrameIndex() const {
		assert(m_isFrameStarted && "Cannot get frame index when frame is not in progress");
		return m_currentFrameIndex;
	}

	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer p_commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer p_commandBuffer);
private:
	Window& m_window;
	Device& m_device;

	std::unique_ptr<SwapChain> m_swapChain;
	std::vector<VkCommandBuffer> m_commandBuffers;

	uint32_t m_currentImageIndex;
	int m_currentFrameIndex = 0;
	bool m_isFrameStarted = false;

	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();
};

} // FFL

#endif // RENDERER_HPP
