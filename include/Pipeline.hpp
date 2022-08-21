#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <string>
#include <vector>

namespace FFL {

class Pipeline {
public:
	Pipeline(const std::string& p_vertPath, const std::string& p_fragPath);
private:
	static std::vector<char> readFile(const std::string& p_filePath);

	void createGraphicsPipeline(const std::string& p_vertPath, const std::string& p_fragPath);
};

}

#endif // PIPELINE_HPP
