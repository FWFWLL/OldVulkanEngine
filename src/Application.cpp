#include "Application.hpp"
#include "GLFW/glfw3.h"

namespace FFL {

void Application::run() {
	while(!window.shouldClose()) {
		glfwPollEvents();
	}
}

} // FFL
