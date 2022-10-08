#include "Window.hpp"

// Libraries
#include <GLFW/glfw3.h>

// STD
#include <cstdint>
#include <stdexcept>

namespace FFL {

Window::Window(uint32_t p_w, uint32_t p_h, std::string p_title) : m_width{p_w}, m_height{p_h}, m_title{p_title} {
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* p_window, int p_width, int p_height) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window));
	window->m_framebufferResized = true;
	window->m_width = p_width;
	window->m_height = p_height;
}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if(m_window == NULL) {
		throw std::runtime_error("failed to create window!");
	}

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void Window::createWindowSurface(VkInstance p_instance, VkSurfaceKHR* p_surface) {
	if(glfwCreateWindowSurface(p_instance, m_window, nullptr, p_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

} // FFL
