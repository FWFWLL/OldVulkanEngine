#ifndef WINDOW_HPP
#define WINDOW_HPP

// Libraries
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// STD
#include <cstdint>
#include <string>

namespace FFL {

class Window {
public:
	Window(uint32_t p_w, uint32_t p_h, std::string p_title);
	~Window();

	// Delete copy-constructors
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool shouldClose() const {return glfwWindowShouldClose(m_window);}
	VkExtent2D getExtent() const {return {m_width, m_height};}
	bool wasWindowResized() const {return m_framebufferResized;}
	GLFWwindow* getGLFWwindow() const {return m_window;}

	void resetWindowResizedFlag() {m_framebufferResized = false;}

	void createWindowSurface(VkInstance p_instance, VkSurfaceKHR* p_surface);
private:
	uint32_t m_width;
	uint32_t m_height;

	bool m_framebufferResized = false;

	std::string m_title;

	GLFWwindow* m_window;

	static void framebufferResizeCallback(GLFWwindow* p_window, int p_width, int p_height);

	void initWindow();
};

} // FFL

#endif // WINDOW_HPP
