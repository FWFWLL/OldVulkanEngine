#ifndef KEYBOARDMOVEMENTCONTROLLER_HPP
#define KEYBOARDMOVEMENTCONTROLLER_HPP

#include "GameObject.hpp"
#include "Window.hpp"

namespace FFL {

class KeyboardMovementController {
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		void moveInPlaneXZ(GLFWwindow* p_window, float p_deltaTime, GameObject& p_gameObject);

		KeyMappings keys{};
		float moveSpeed{3.0f};
		float lookSpeed{1.5f};
};

}

#endif // KEYBOARDMOVEMENTCONTROLLER_HPP
