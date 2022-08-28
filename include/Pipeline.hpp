#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Device.hpp"

namespace FFL {

struct PipelineConfigInfo {
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
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
private:
	Device& m_device; // Unsafe
	VkPipeline m_graphicsPipeline;
	VkShaderModule m_vertShaderModule;
	VkShaderModule m_fragShaderModule;
private:
	static std::vector<char> readFile(const std::string& p_filePath);

	void createGraphicsPipeline(const PipelineConfigInfo& p_configInfo, const std::string& p_vertPath, const std::string& p_fragPath);
	void createShaderModule(const std::vector<char>& p_code, VkShaderModule* p_shaderModule);
public:
	static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t p_w, uint32_t p_h);

	void bind(VkCommandBuffer p_commandBuffer);
};

}

#endif // PIPELINE_HPP
