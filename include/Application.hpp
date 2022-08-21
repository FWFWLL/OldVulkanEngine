#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "Pipeline.hpp"

namespace FFL {

class Application {
private:
	Window m_window{WIDTH, HEIGHT, "Vulkan_C++"};
	Pipeline m_pipeline{"shaders/shader.vert.spv", "shaders/shader.frag.spv"};
public:
	static constexpr int WIDTH = 640;
	static constexpr int HEIGHT = 640;
public:
	void run();
};

}

#endif // APPLICATION_HPP
