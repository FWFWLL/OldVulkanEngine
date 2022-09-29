#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Device.hpp"

#include <memory>

namespace FFL {

class SwapChain {
public:
	SwapChain(Device& p_device, VkExtent2D p_extent);
	SwapChain(Device& p_device, VkExtent2D p_extent, std::shared_ptr<SwapChain> p_previous);
	~SwapChain();

	// Delete copy-constructor
	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;
public:
	static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;
private:
	Device& m_device;
	VkExtent2D m_windowExtent;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	VkRenderPass m_renderPass;

	std::vector<VkImage> m_depthImages;
	std::vector<VkDeviceMemory> m_depthImageMemories;
	std::vector<VkImageView> m_depthImageViews;
	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;

	VkSwapchainKHR m_swapChain;
	std::shared_ptr<SwapChain> m_oldSwapChain;

	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;
	uint32_t m_currentFrame = 0;
private:
	void init();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDepthResources();
	void createFramebuffers();
	void createSyncObjects();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& p_availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& p_availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& p_capabilities);
public:
	VkResult acquireNextImage(uint32_t* p_imageIndex);
	VkFormat findDepthFormat();
	VkResult submitCommandBuffers(const VkCommandBuffer* p_buffers, uint32_t* p_imageIndex);
public:
	VkFramebuffer getFramebuffer(int p_index) {return m_swapChainFramebuffers[p_index];}
	VkRenderPass getRenderPass() {return m_renderPass;}
	VkImageView getImageView(int p_index) {return m_swapChainImageViews[p_index];}
	size_t imageCount() {return m_swapChainImages.size();}
	VkFormat getSwapChainImageFormat() {return m_swapChainImageFormat;}
	VkExtent2D getSwapChainExtent() {return m_swapChainExtent;}
	uint32_t width() {return m_swapChainExtent.width;}
	uint32_t height() {return m_swapChainExtent.height;}
	float extentAspectRatio() {return static_cast<float>(width()) / static_cast<float>(height());}
};

} // FFL

#endif // SWAPCHAIN_HPP
