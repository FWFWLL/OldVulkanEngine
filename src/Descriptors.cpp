#include "Descriptors.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace FFL {

DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::addBinding(uint32_t p_binding, VkDescriptorType p_descriptorType, VkShaderStageFlags p_stageFlags, uint32_t p_descriptorCount) {
	assert(m_bindings.count(p_binding) == 0 && "Binding is already in use");

	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = p_binding;
	layoutBinding.descriptorType = p_descriptorType;
	layoutBinding.descriptorCount = p_descriptorCount;
	layoutBinding.stageFlags = p_stageFlags;
	m_bindings[p_binding] = layoutBinding;

	return *this;
}

DescriptorSetLayout::DescriptorSetLayout(Device& p_device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_bindings) : m_device{p_device}, m_bindings{p_bindings} {
	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
	for(auto kv : p_bindings) {
		setLayoutBindings.push_back(kv.second);
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

	if(vkCreateDescriptorSetLayout(m_device.device(), &descriptorSetLayoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

DescriptorSetLayout::~DescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
}

DescriptorPool::Builder& DescriptorPool::Builder::addPoolSize(VkDescriptorType p_descriptorType, uint32_t p_count) {
	m_poolSizes.push_back({p_descriptorType, p_count});

	return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags p_poolFlags) {
	m_poolFlags = p_poolFlags;

	return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setMaxSets(uint32_t p_count) {
	m_maxSets = p_count;

	return *this;
}

DescriptorPool::DescriptorPool(Device& p_device, uint32_t p_maxSets, VkDescriptorPoolCreateFlags p_poolFlags, const std::vector<VkDescriptorPoolSize>& p_poolSizes) : m_device{p_device} {
	VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(p_poolSizes.size());
	descriptorPoolInfo.pPoolSizes = p_poolSizes.data();
	descriptorPoolInfo.maxSets = p_maxSets;
	descriptorPoolInfo.flags = p_poolFlags;

	if(vkCreateDescriptorPool(p_device.device(), &descriptorPoolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

DescriptorPool::~DescriptorPool() {
	vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr);
}

bool DescriptorPool::allocateDescriptor(const VkDescriptorSetLayout p_descriptorSetLayout, VkDescriptorSet& p_descriptor) const {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.pSetLayouts = &p_descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	if(vkAllocateDescriptorSets(m_device.device(), &allocInfo, &p_descriptor) != VK_SUCCESS) {
		return false;
	}

	return true;
}

void DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& p_descriptors) const {
	vkFreeDescriptorSets(m_device.device(), m_descriptorPool, static_cast<uint32_t>(p_descriptors.size()), p_descriptors.data());
}

void DescriptorPool::resetPool() {
	vkResetDescriptorPool(m_device.device(), m_descriptorPool, 0);
}

DescriptorWriter::DescriptorWriter(DescriptorSetLayout& p_setLayout, DescriptorPool& p_pool) : m_setLayout{p_setLayout}, m_pool{p_pool} {}

DescriptorWriter& DescriptorWriter::writeBuffer(uint32_t p_binding, VkDescriptorBufferInfo* p_bufferInfo) {
	assert(m_setLayout.m_bindings.count(p_binding) == 1 && "Layout does not contain specified binding");

	VkDescriptorSetLayoutBinding& bindingDescription = m_setLayout.m_bindings[p_binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = p_binding;
	write.pBufferInfo = p_bufferInfo;
	write.descriptorCount = 1;

	m_writes.push_back(write);

	return *this;
}

DescriptorWriter& DescriptorWriter::writeImage(uint32_t p_binding, VkDescriptorImageInfo* p_imageInfo) {
	assert(m_setLayout.m_bindings.count(p_binding) == 1 && "Layout does not contain specified binding");

	VkDescriptorSetLayoutBinding& bindingDescription = m_setLayout.m_bindings[p_binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = p_binding;
	write.pImageInfo = p_imageInfo;
	write.descriptorCount = 1;

	m_writes.push_back(write);

	return *this;
}

bool DescriptorWriter::build(VkDescriptorSet& p_set) {
	bool success = m_pool.allocateDescriptor(m_setLayout.getDescriptorSetLayout(), p_set);

	if(!success) {
		return false;
	}

	overwrite(p_set);

	return true;
}

void DescriptorWriter::overwrite(VkDescriptorSet& p_set) {
	for(VkWriteDescriptorSet& write : m_writes) {
		write.dstSet = p_set;
	}

	vkUpdateDescriptorSets(m_pool.m_device.device(), m_writes.size(), m_writes.data(), 0, nullptr);
}

} // FFL
