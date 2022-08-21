#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"

namespace FFL {

class Application {
private:
	Window window{WIDTH, HEIGHT, "Vulkan_C++"};
public:
	static constexpr int WIDTH = 640;
	static constexpr int HEIGHT = 640;
public:
	void run();
};

}

#endif // APPLICATION_HPP
