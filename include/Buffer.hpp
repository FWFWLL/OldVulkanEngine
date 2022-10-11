#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Device.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <cstdint>

namespace FFL {

class Buffer {
public:
	Buffer(Device& p_device, VkDeviceSize p_instanceSize, uint32_t p_instanceCount, VkBufferUsageFlags p_usageFlags, VkMemoryPropertyFlags p_memoryPropertyFlags, VkDeviceSize p_minOffsetAlignment = 1);
	~Buffer();

	// Delete copy constructor
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	void* getMappedMemory() const {return m_mapped;}
	VkBuffer getBuffer() const {return m_buffer;}
	VkDeviceSize getInstanceSize() const {return m_instanceSize;}
	uint32_t getInstanceCount() const {return m_instanceCount;}
	VkBufferUsageFlags getUsageFlags() const {return m_usageFlags;}
	VkDeviceSize getBufferSize() const {return m_bufferSize;}
	VkMemoryPropertyFlags getMemoryPropertyFlags() const {return m_memoryPropertyFlags;}
	VkDeviceSize getAlignmentSize() const {return m_alignmentSize;}

	VkResult map(VkDeviceSize p_size = VK_WHOLE_SIZE, VkDeviceSize p_offset = 0);
	void unmap();

	void writeToBuffer(void* p_data, VkDeviceSize p_size = VK_WHOLE_SIZE, VkDeviceSize p_offset = 0);
	VkResult flush(VkDeviceSize p_size = VK_WHOLE_SIZE, VkDeviceSize p_offset = 0);
	VkDescriptorBufferInfo descriptorInfo(VkDeviceSize p_size = VK_WHOLE_SIZE, VkDeviceSize p_offset = 0);
	VkResult invalidate(VkDeviceSize p_size = VK_WHOLE_SIZE, VkDeviceSize p_offset = 0);

	void writeToIndex(void* p_data, int p_index);
	VkResult flushIndex(int p_index);
	VkDescriptorBufferInfo descriptorInfoForIndex(int p_index);
	VkResult invalidateIndex(int p_index);
private:
	void* m_mapped = nullptr;
	VkBuffer m_buffer = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;

	Device& m_device;
	VkDeviceSize m_instanceSize;
	uint32_t m_instanceCount;
	VkBufferUsageFlags m_usageFlags;
	VkDeviceSize m_bufferSize;
	VkMemoryPropertyFlags m_memoryPropertyFlags;
	VkDeviceSize m_alignmentSize;

	static VkDeviceSize getAlignment(VkDeviceSize p_instanceSize, VkDeviceSize p_minOffsetAlignment);
};

} // FFL

#endif // BUFFER_HPP
