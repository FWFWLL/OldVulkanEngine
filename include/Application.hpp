#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"

// STD
#include <memory>
#include <vector>

namespace FFL {

class Application {
public:
	static constexpr uint32_t SCREEN_WIDTH = 640;
	static constexpr uint32_t SCREEN_HEIGHT = 640;

	Application();
	~Application();

	// Delete copy-constructor
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void run();
private:
	Window m_window{SCREEN_WIDTH, SCREEN_HEIGHT, "Vulkan_C++"};
	Device m_device{m_window};
	Renderer m_renderer{m_window, m_device};

	std::vector<GameObject> m_gameObjects;

	void loadGameObjects();
};

}

#endif // APPLICATION_HPP
