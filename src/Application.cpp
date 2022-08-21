#include "Application.hpp"

namespace FFL {

void Application::run() {
	while(!m_window.shouldClose()) {
		glfwPollEvents();
	}
}

} // FFL
