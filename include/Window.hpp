#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace FFL {

class Window {
private:
	const int m_width;
	const int m_height;

	std::string m_title;

	GLFWwindow* m_window;
public:
	Window(int p_w, int p_h, std::string p_title);
	~Window();

	Window(const Window&) = delete;
	Window &operator=(const Window&) = delete;

	bool shouldClose() {return glfwWindowShouldClose(m_window);}
private:
	void initWindow();
};

} // FFL

#endif // WINDOW_HPP
