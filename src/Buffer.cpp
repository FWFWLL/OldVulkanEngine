#include "Buffer.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <cassert>
#include <cstring>

namespace FFL {

Buffer::Buffer(Device& p_device, VkDeviceSize p_instanceSize, uint32_t p_instanceCount, VkBufferUsageFlags p_usageFlags, VkMemoryPropertyFlags p_memoryPropertyFlags, VkDeviceSize p_minOffsetAlignment) : m_device{p_device}, m_instanceSize{p_instanceSize}, m_instanceCount{p_instanceCount}, m_usageFlags{p_usageFlags}, m_memoryPropertyFlags{p_memoryPropertyFlags} {
	m_alignmentSize = getAlignment(p_instanceSize, p_minOffsetAlignment);
	m_bufferSize = m_alignmentSize * p_instanceCount;

	m_device.createBuffer(m_bufferSize, p_usageFlags, p_memoryPropertyFlags, m_buffer, m_memory);
}

Buffer::~Buffer() {
	unmap();
	vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
	vkFreeMemory(m_device.device(), m_memory, nullptr);
}

VkResult Buffer::map(VkDeviceSize p_size, VkDeviceSize p_offset) {
	assert(m_buffer && m_memory && "Called map on buffer before creation");

	return vkMapMemory(m_device.device(), m_memory, p_offset, p_size, 0, &m_mapped);
}

void Buffer::unmap() {
	if(m_mapped) {
		vkUnmapMemory(m_device.device(), m_memory);
		m_mapped = nullptr;
	}
}

void Buffer::writeToBuffer(void* p_data, VkDeviceSize p_size, VkDeviceSize p_offset) {
	assert(m_mapped && "Can't copy to unmapped buffer");

	if(p_size == VK_WHOLE_SIZE) {
		memcpy(m_mapped, p_data, m_bufferSize);
	} else {
		char* memOffset = static_cast<char*>(m_mapped);
		memOffset += p_offset;
		memcpy(memOffset, p_data, p_size);
	}
}

VkResult Buffer::flush(VkDeviceSize p_size, VkDeviceSize p_offset) {
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_memory;
	mappedRange.offset = p_offset;
	mappedRange.size = p_size;

	return vkFlushMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize p_size, VkDeviceSize p_offset) {
	return VkDescriptorBufferInfo{m_buffer, p_offset, p_size};
}

VkResult Buffer::invalidate(VkDeviceSize p_size, VkDeviceSize p_offset) {
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_memory;
	mappedRange.offset = p_offset;
	mappedRange.size = p_size;

	return vkInvalidateMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

void Buffer::writeToIndex(void* p_data, int p_index) {
	writeToBuffer(p_data, m_instanceSize, p_index * m_alignmentSize);
}

VkResult Buffer::flushIndex(int p_index) {
	return flush(m_alignmentSize, p_index * m_alignmentSize);
}

VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int p_index) {
	return descriptorInfo(m_alignmentSize, p_index * m_alignmentSize);
}

VkResult Buffer::invalidateIndex(int p_index) {
	return invalidate(m_alignmentSize, p_index * m_alignmentSize);
}

VkDeviceSize Buffer::getAlignment(VkDeviceSize p_instanceSize, VkDeviceSize p_minOffsetAlignment) {
	if(p_minOffsetAlignment > 0) {
		return (p_instanceSize + p_minOffsetAlignment - 1) & ~(p_minOffsetAlignment - 1);
	}

	return p_instanceSize;
}

} // FFL
