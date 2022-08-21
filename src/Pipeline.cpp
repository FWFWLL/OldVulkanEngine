#include "Pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace FFL {

Pipeline::Pipeline(const std::string& p_vertPath, const std::string& p_fragPath) {
	createGraphicsPipeline(p_vertPath, p_fragPath);
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

void Pipeline::createGraphicsPipeline(const std::string& p_vertPath, const std::string& p_fragPath) {
	auto vertShaderCode = readFile(p_vertPath);
	auto fragShaderCode = readFile(p_fragPath);

	std::cout << "Vertex Shader Code Size: " << vertShaderCode.size() << '\n';
	std::cout << "Fragment Shader Code Size: " << fragShaderCode.size() << '\n';
}

} // FFL
