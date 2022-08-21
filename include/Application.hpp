#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "Device.hpp"
#include "Pipeline.hpp"

namespace FFL {

class Application {
public:
	static const uint32_t SCREEN_WIDTH = 640;
	static const uint32_t SCREEN_HEIGHT = 640;
private:
	Window m_window = {SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan_C++"};
	Device m_device = {m_window};
	Pipeline m_pipeline = {m_device, Pipeline::defaultPipelineConfigInfo(SCREEN_WIDTH, SCREEN_HEIGHT), "shaders/shader.vert.spv", "shaders/shader.frag.spv"};
public:
	void run();
};

}

#endif // APPLICATION_HPP
