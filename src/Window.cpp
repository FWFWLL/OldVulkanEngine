#include "Window.hpp"

#include <GLFW/glfw3.h>

namespace FFL {

Window::Window(int p_w, int p_h, std::string p_title) : m_width{p_w}, m_height{p_h}, m_title{p_title} {
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}

} // FFL
