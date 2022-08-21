#include "Pipeline.hpp"
#include "Device.hpp"
#include "vulkan/vulkan_core.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace FFL {

Pipeline::Pipeline(Device& p_device, const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath) : m_device{p_device} {
	createGraphicsPipeline(p_configInfo, p_vertPath, p_fragPath);
}

std::vector<char> Pipeline::readFile(const std::string& p_filePath) {
	std::ifstream file(p_filePath, std::ios::ate | std::ios::binary);

	if(!file.is_open()) {
		throw std::runtime_error("failed to open file: " + p_filePath);
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void Pipeline::createGraphicsPipeline(const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath) {
	auto vertShaderCode = readFile(p_vertPath);
	auto fragShaderCode = readFile(p_fragPath);

	std::cout << "Vertex Shader Code Size: " << vertShaderCode.size() << '\n';
	std::cout << "Fragment Shader Code Size: " << fragShaderCode.size() << '\n';
}

void Pipeline::createShaderModule(const std::vector<char>& p_code, VkShaderModule* p_shaderModule) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = p_code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(p_code.data());

	if(vkCreateShaderModule(m_device.device(), &createInfo, nullptr, p_shaderModule)!= VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}


PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t p_w, uint32_t p_h) {
	PipelineConfigInfo configInfo = {};

	return configInfo;
}

} // FFL
