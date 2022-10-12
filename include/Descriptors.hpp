#ifndef DESCRIPTORS_HPP
#define DESCRIPTORS_HPP

#include "Device.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <memory>
#include <unordered_map>
#include <vector>

namespace FFL {

class DescriptorSetLayout {
public:
	class Builder {
	public:
		Builder(Device& p_device) : m_device{p_device} {}

		std::unique_ptr<DescriptorSetLayout> build() const {return std::make_unique<DescriptorSetLayout>(m_device, m_bindings);}

		Builder& addBinding(uint32_t p_binding, VkDescriptorType p_descriptorType, VkShaderStageFlags p_stageFlags, uint32_t p_descriptorCount = 1);
	private:
		Device& m_device;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings = {};
	};

	DescriptorSetLayout(Device& p_device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_bindings);
	~DescriptorSetLayout();

	// Delete copy-constructor
	DescriptorSetLayout(const DescriptorSetLayout&) = delete;
	DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

	VkDescriptorSetLayout getDescriptorSetLayout() const {return m_descriptorSetLayout;}
private:
	Device& m_device;
	VkDescriptorSetLayout m_descriptorSetLayout;
	std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

	friend class DescriptorWriter;
};

class DescriptorPool {
public:
	class Builder {
	public:
		Builder(Device& p_device) : m_device{p_device} {}

		std::unique_ptr<DescriptorPool> build() const {return std::make_unique<DescriptorPool>(m_device, m_maxSets, m_poolFlags, m_poolSizes);}

		Builder& setMaxSets(uint32_t p_count);
		Builder& setPoolFlags(VkDescriptorPoolCreateFlags p_poolFlags);
		Builder& addPoolSize(VkDescriptorType p_descriptorType, uint32_t p_count);
	private:
		Device& m_device;
		uint32_t m_maxSets = 1000;
		VkDescriptorPoolCreateFlags m_poolFlags = 0;
		std::vector<VkDescriptorPoolSize> m_poolSizes = {};
	};

	DescriptorPool(Device& p_device, uint32_t p_maxSets, VkDescriptorPoolCreateFlags p_poolFlags, const std::vector<VkDescriptorPoolSize>& p_poolSizes);
	~DescriptorPool();

	// Delete copy-constructor
	DescriptorPool(const DescriptorPool&) = delete;
	DescriptorPool& operator=(const DescriptorPool&) = delete;

	bool allocateDescriptor(const VkDescriptorSetLayout p_descriptorSetLayout, VkDescriptorSet& p_descriptor) const;
	void freeDescriptors(std::vector<VkDescriptorSet>& p_descriptors) const;

	void resetPool();
private:
	Device& m_device;
	VkDescriptorPool m_descriptorPool;

	friend class DescriptorWriter;
};

class DescriptorWriter {
public:
	DescriptorWriter(DescriptorSetLayout& p_setLayout, DescriptorPool& p_pool);

	DescriptorWriter& writeBuffer(uint32_t p_binding, VkDescriptorBufferInfo* p_bufferInfo);
	DescriptorWriter& writeImage(uint32_t p_binding, VkDescriptorImageInfo* p_imageInfo);

	bool build(VkDescriptorSet& p_set);
	void overwrite(VkDescriptorSet& p_set);
private:
	DescriptorSetLayout& m_setLayout;
	DescriptorPool& m_pool;
	std::vector<VkWriteDescriptorSet> m_writes;
};

} // FFL

#endif // DESCRIPTORS_HPP
