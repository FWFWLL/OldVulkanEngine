#include "KeyboardMovementController.hpp"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include <limits>

namespace FFL {

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* p_window, float p_deltaTime, GameObject& p_gameObject) {
	glm::vec3 rotate{0.0f};
	if(glfwGetKey(p_window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
	if(glfwGetKey(p_window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
	if(glfwGetKey(p_window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
	if(glfwGetKey(p_window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

	if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
		p_gameObject.transform.rotation += lookSpeed * p_deltaTime * glm::normalize(rotate);
	}

	// Limit pitch values between about +/- ~85 degrees
	p_gameObject.transform.rotation.x = glm::clamp(p_gameObject.transform.rotation.x, -1.5f, 1.5f);
	p_gameObject.transform.rotation.y = glm::mod(p_gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = p_gameObject.transform.rotation.y;
	const glm::vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
	const glm::vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
	const glm::vec3 upDir{0.0f, -1.0f, 0.0f};

	glm::vec3 moveDir{0.0f};
	if(glfwGetKey(p_window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if(glfwGetKey(p_window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if(glfwGetKey(p_window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if(glfwGetKey(p_window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if(glfwGetKey(p_window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if(glfwGetKey(p_window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
		p_gameObject.transform.translation += moveSpeed * p_deltaTime * glm::normalize(moveDir);
	}
}

}
