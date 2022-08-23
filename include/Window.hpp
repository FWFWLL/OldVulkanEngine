#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace FFL {

class Window {
public:
	Window(uint32_t p_w, uint32_t p_h, std::string p_title);
	~Window();

	// Delete copy-constructors
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	const uint32_t m_width;
	const uint32_t m_height;

	std::string m_title;

	GLFWwindow* m_window;
private:
	void initWindow();
public:
	void createWindowSurface(VkInstance p_instance, VkSurfaceKHR* p_surface);
public:
	bool shouldClose() {return glfwWindowShouldClose(m_window);}
	VkExtent2D getExtent() {return {m_width, m_height};}
};

} // FFL

#endif // WINDOW_HPP
