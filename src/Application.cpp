#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace FFL {

void Application::run() {
	while(!m_window.shouldClose()) {
		glfwPollEvents();
	}
}

} // FFL
