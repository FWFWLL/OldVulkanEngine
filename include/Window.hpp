#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace FFL {

class Window {
public:
	Window(int p_w, int p_h, std::string p_title);
	~Window();

	// Delete copy-constructors
	Window(const Window&) = delete;
	Window &operator=(const Window&) = delete;
private:
	GLFWwindow* m_window;
	
	const int m_width;
	const int m_height;

	std::string m_title;
private:
	void initWindow();
public:
	bool shouldClose() {return glfwWindowShouldClose(m_window);}

	void createWindowSurface(VkInstance p_instance, VkSurfaceKHR* p_surface);
};

} // FFL

#endif // WINDOW_HPP
