#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Window.hpp"

// STD
#include <optional>
#include <vector>

namespace FFL {

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {return graphicsFamily.has_value() && presentFamily.has_value();}
};

class Device {
public:
	Device(Window& p_window);
	~Device();

	// Delete copy-constructor
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;

	// Delete move-constructor
	Device(Device&&) = delete;
	Device& operator=(Device&&) = delete;

	VkPhysicalDeviceProperties properties;
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	VkDevice device() {return m_device;}
	VkSurfaceKHR surface() {return m_surface;}
	VkQueue graphicsQueue() {return m_graphicsQueue;}
	VkQueue presentQueue() {return m_presentQueue;}
	VkCommandPool getCommandPool() {return m_commandPool;}
	QueueFamilyIndices findPhysicalQueueFamilies() {return findQueueFamilies(m_physicalDevice);}
	SwapChainSupportDetails getSwapChainSupport() {return querySwapChainSupport(m_physicalDevice);}

	uint32_t findMemoryType(uint32_t p_typeFilter, VkMemoryPropertyFlags p_properties);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& p_candidates, VkImageTiling p_tiling, VkFormatFeatureFlags p_features);
	void createBuffer(VkDeviceSize p_size, VkBufferUsageFlags p_usage, VkMemoryPropertyFlags p_properties, VkBuffer& p_buffer, VkDeviceMemory& p_bufferMemory);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer p_commandBuffer);
	void copyBuffer(VkBuffer p_src, VkBuffer p_dst, VkDeviceSize p_size);
	void copyBufferToImage(VkBuffer p_buffer, VkImage p_image, uint32_t p_w, uint32_t p_h, uint32_t p_layerCount);
	void createImageWithInfo(const VkImageCreateInfo& p_imageInfo, VkMemoryPropertyFlags p_properties, VkImage& p_image, VkDeviceMemory& p_imageMemory);
private:
	const std::vector<const char*> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
	const std::vector<const char*> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	Window& m_window;

	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkSurfaceKHR m_surface;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkCommandPool m_commandPool;

	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createCommandPool();

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& p_createInfo);
	void hasGLFWRequiredInstanceExtensions();
	bool isDeviceSuitable(VkPhysicalDevice p_device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice p_device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice p_device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice p_device);
};

} // FFL

#endif // DEVICE_HPP
