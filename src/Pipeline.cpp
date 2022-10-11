#include "Pipeline.hpp"
#include "Model.hpp"

// Libraries
#include <array>
#include <vulkan/vulkan_core.h>

// STD
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>

namespace FFL {

Pipeline::Pipeline(Device& p_device, const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath) : m_device{p_device} {
	createGraphicsPipeline(p_configInfo, p_vertPath, p_fragPath);
}

Pipeline::~Pipeline() {
	vkDestroyShaderModule(m_device.device(), m_vertShaderModule, nullptr);
	vkDestroyShaderModule(m_device.device(), m_fragShaderModule, nullptr);
	vkDestroyPipeline(m_device.device(), m_graphicsPipeline, nullptr);
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
	assert(p_configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in p_configInfo");
	assert(p_configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in p_configInfo");

	std::vector<char> vertShaderCode = readFile(p_vertPath);
	std::vector<char> fragShaderCode = readFile(p_fragPath);

	std::cout << "Vertex Shader Code Size: " << vertShaderCode.size() << '\n';
	std::cout << "Fragment Shader Code Size: " << fragShaderCode.size() << '\n';

	createShaderModule(vertShaderCode, &m_vertShaderModule);
	createShaderModule(fragShaderCode, &m_fragShaderModule);

	VkPipelineShaderStageCreateInfo shaderStages[2];
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = m_vertShaderModule;
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].pSpecializationInfo = nullptr;

	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = m_fragShaderModule;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pNext = nullptr;
	shaderStages[1].pSpecializationInfo = nullptr;

	std::vector<VkVertexInputBindingDescription> bindingDescriptions = Model::Vertex::getBindingDescriptions();
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = Model::Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &p_configInfo.inputAssemblyInfo;
	pipelineInfo.pViewportState = &p_configInfo.viewportInfo;
	pipelineInfo.pRasterizationState = &p_configInfo.rasterizationInfo;
	pipelineInfo.pMultisampleState = &p_configInfo.multisampleInfo;
	pipelineInfo.pColorBlendState = &p_configInfo.colorBlendInfo;
	pipelineInfo.pDepthStencilState = &p_configInfo.depthStencilInfo;
	pipelineInfo.pDynamicState = &p_configInfo.dynamicStateInfo;
	pipelineInfo.layout = p_configInfo.pipelineLayout;
	pipelineInfo.renderPass = p_configInfo.renderPass;
	pipelineInfo.subpass = p_configInfo.subpass;
	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if(vkCreateGraphicsPipelines(m_device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
}

void Pipeline::createShaderModule(const std::vector<char>& p_code, VkShaderModule* p_shaderModule) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = p_code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(p_code.data());

	if(vkCreateShaderModule(m_device.device(), &createInfo, nullptr, p_shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}

void Pipeline::defaultPipelineConfigInfo(PipelineConfigInfo& p_configInfo) {
	p_configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	p_configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	p_configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	p_configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	p_configInfo.viewportInfo.viewportCount = 1;
	p_configInfo.viewportInfo.pViewports = nullptr;
	p_configInfo.viewportInfo.scissorCount = 1;
	p_configInfo.viewportInfo.pScissors = nullptr;

	p_configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	p_configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
	p_configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	p_configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	p_configInfo.rasterizationInfo.lineWidth = 1.0f;
	p_configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	p_configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	p_configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	p_configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
	p_configInfo.rasterizationInfo.depthBiasClamp = 0.0f;          // Optional
	p_configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;    // Optional

	p_configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	p_configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	p_configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	p_configInfo.multisampleInfo.minSampleShading = 1.0f;          // Optional
	p_configInfo.multisampleInfo.pSampleMask = nullptr;            // Optional
	p_configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	p_configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;      // Optional

	p_configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	p_configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
	p_configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	p_configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	p_configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
	p_configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	p_configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	p_configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

	p_configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	p_configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	p_configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	p_configInfo.colorBlendInfo.attachmentCount = 1;
	p_configInfo.colorBlendInfo.pAttachments = &p_configInfo.colorBlendAttachment;
	p_configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
	p_configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
	p_configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
	p_configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

	p_configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	p_configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	p_configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	p_configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	p_configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	p_configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
	p_configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
	p_configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	p_configInfo.depthStencilInfo.front = {}; // Optional
	p_configInfo.depthStencilInfo.back = {};  // Optional

	p_configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	p_configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	p_configInfo.dynamicStateInfo.pDynamicStates = p_configInfo.dynamicStateEnables.data();
	p_configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(p_configInfo.dynamicStateEnables.size());
	p_configInfo.dynamicStateInfo.flags = 0;
}

void Pipeline::bind(VkCommandBuffer p_commandBuffer) {
	vkCmdBindPipeline(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
}

} // FFL
