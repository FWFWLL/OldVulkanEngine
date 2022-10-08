#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Device.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

// STD
#include <vector>

namespace FFL {

struct PipelineConfigInfo {
	// Delete copy-constructors
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class Pipeline {
public:
	Pipeline(Device& p_device, const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath);
	~Pipeline();

	// Delete copy-constructors
	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;

	static void defaultPipelineConfigInfo(PipelineConfigInfo& p_configInfo);

	void bind(VkCommandBuffer p_commandBuffer);
private:
	Device& m_device;
	VkPipeline m_graphicsPipeline;
	VkShaderModule m_vertShaderModule;
	VkShaderModule m_fragShaderModule;

	static std::vector<char> readFile(const std::string& p_filePath);

	void createGraphicsPipeline(const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath);
	void createShaderModule(const std::vector<char>& p_code, VkShaderModule* p_shaderModule);
};

} // FFL

#endif // PIPELINE_HPP
