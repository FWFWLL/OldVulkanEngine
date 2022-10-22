#include "Camera.hpp"

// STD
#include <cassert>
#include <cmath>
#include <limits>

// Libraries
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

namespace FFL {

void Camera::setOrthographicProjection(float p_left, float p_right, float p_top, float p_bottom, float p_near, float p_far) {
	m_projectionMatrix = glm::mat4{1.0f};
	m_projectionMatrix[0][0] = 2.0f / (p_right - p_left);
	m_projectionMatrix[1][1] = 2.0f / (p_bottom - p_top);
	m_projectionMatrix[2][2] = 1.0f / (p_far - p_near);
	m_projectionMatrix[3][0] = -(p_right + p_left) / (p_right - p_left);
	m_projectionMatrix[3][1] = -(p_bottom + p_top) / (p_bottom - p_top);
	m_projectionMatrix[3][2] = -p_near / (p_far - p_near);
}

void Camera::setPerspectiveProjection(float p_fovY, float p_aspect, float p_near, float p_far) {
	assert(glm::abs(p_aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	
	const float tanHalfFovY = tan(p_fovY / 2.0f);

	m_projectionMatrix = glm::mat4{0.0f};
	m_projectionMatrix[0][0] = 1.0f / (p_aspect * tanHalfFovY);
	m_projectionMatrix[1][1] = 1.0f / tanHalfFovY;
	m_projectionMatrix[2][2] = p_far / (p_far - p_near);
	m_projectionMatrix[2][3] = 1.0f;
	m_projectionMatrix[3][2] = -(p_far * p_near) / (p_far - p_near);
}

void Camera::setViewDirection(glm::vec3 p_position, glm::vec3 p_direction, glm::vec3 p_up) {
	const glm::vec3 w{glm::normalize(p_direction)};
	const glm::vec3 u{glm::normalize(glm::cross(w, p_up))};
	const glm::vec3 v{glm::cross(w, u)};

	m_viewMatrix = glm::mat4{1.0f};
	m_viewMatrix[0][0] = u.x;
	m_viewMatrix[1][0] = u.y;
	m_viewMatrix[2][0] = u.z;
	m_viewMatrix[0][1] = v.x;
	m_viewMatrix[1][1] = v.y;
	m_viewMatrix[2][1] = v.z;
	m_viewMatrix[0][2] = w.x;
	m_viewMatrix[1][2] = w.y;
	m_viewMatrix[2][2] = w.z;
	m_viewMatrix[3][0] = -glm::dot(u, p_position);
	m_viewMatrix[3][1] = -glm::dot(v, p_position);
	m_viewMatrix[3][2] = -glm::dot(w, p_position);

	m_inverseViewMatrix = glm::mat4{1.f};
	m_inverseViewMatrix[0][0] = u.x;
	m_inverseViewMatrix[0][1] = u.y;
	m_inverseViewMatrix[0][2] = u.z;
	m_inverseViewMatrix[1][0] = v.x;
	m_inverseViewMatrix[1][1] = v.y;
	m_inverseViewMatrix[1][2] = v.z;
	m_inverseViewMatrix[2][0] = w.x;
	m_inverseViewMatrix[2][1] = w.y;
	m_inverseViewMatrix[2][2] = w.z;
	m_inverseViewMatrix[3][0] = p_position.x;
	m_inverseViewMatrix[3][1] = p_position.y;
	m_inverseViewMatrix[3][2] = p_position.z;
}

void Camera::setViewTarget(glm::vec3 p_position, glm::vec3 p_target, glm::vec3 p_up) {
	setViewDirection(p_position, p_target - p_position, p_up);
}

void Camera::setViewYXZ(glm::vec3 p_position, glm::vec3 p_rotation) {
	const float s1 = glm::sin(p_rotation.y);
	const float c1 = glm::cos(p_rotation.y);
	const float s2 = glm::sin(p_rotation.x);
	const float c2 = glm::cos(p_rotation.x);
	const float s3 = glm::sin(p_rotation.z);
	const float c3 = glm::cos(p_rotation.z);

	const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
	const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
	const glm::vec3 w{(c2 * s1), -s2, (c1 * c2)};

	m_viewMatrix = glm::mat4{1.f};
	m_viewMatrix[0][0] = u.x;
	m_viewMatrix[1][0] = u.y;
	m_viewMatrix[2][0] = u.z;
	m_viewMatrix[0][1] = v.x;
	m_viewMatrix[1][1] = v.y;
	m_viewMatrix[2][1] = v.z;
	m_viewMatrix[0][2] = w.x;
	m_viewMatrix[1][2] = w.y;
	m_viewMatrix[2][2] = w.z;
	m_viewMatrix[3][0] = -glm::dot(u, p_position);
	m_viewMatrix[3][1] = -glm::dot(v, p_position);
	m_viewMatrix[3][2] = -glm::dot(w, p_position);

	m_inverseViewMatrix = glm::mat4{1.f};
	m_inverseViewMatrix[0][0] = u.x;
	m_inverseViewMatrix[0][1] = u.y;
	m_inverseViewMatrix[0][2] = u.z;
	m_inverseViewMatrix[1][0] = v.x;
	m_inverseViewMatrix[1][1] = v.y;
	m_inverseViewMatrix[1][2] = v.z;
	m_inverseViewMatrix[2][0] = w.x;
	m_inverseViewMatrix[2][1] = w.y;
	m_inverseViewMatrix[2][2] = w.z;
	m_inverseViewMatrix[3][0] = p_position.x;
	m_inverseViewMatrix[3][1] = p_position.y;
	m_inverseViewMatrix[3][2] = p_position.z;
}

} // FFL
