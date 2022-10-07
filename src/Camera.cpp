#include "Camera.hpp"

// STD
#include <cassert>
#include <cmath>
#include <limits>

// Libraries
#include <glm/common.hpp>
#include <glm/fwd.hpp>

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
	m_projectionMatrix[1][1] = 1.0f / (tanHalfFovY);
	m_projectionMatrix[2][2] = p_far / (p_far - p_near);
	m_projectionMatrix[2][3] = 1.0f;
	m_projectionMatrix[3][2] = -(p_far * p_near) / (p_far - p_near);
}

}
