#ifndef CAMERA_HPP
#define CAMERA_HPP

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace FFL {

class Camera {
	public:
		const glm::mat4& getProjection() const {return m_projectionMatrix;}
		const glm::mat4& getView() const {return m_viewMatrix;}

		void setOrthographicProjection(float p_left, float p_right, float p_top, float p_bottom, float p_near, float p_far);
		void setPerspectiveProjection(float p_fovY, float p_aspect, float p_near, float p_far);
		void setViewDirection(glm::vec3 p_position, glm::vec3 p_direction, glm::vec3 p_up = glm::vec3{0.0f, -1.0f, 0.0f});
		void setViewTarget(glm::vec3 p_position, glm::vec3 p_target, glm::vec3 p_up = glm::vec3{0.0f, -1.0f, 0.0f});
		void setViewYXZ(glm::vec3 p_position, glm::vec3 p_rotation);
	private:
		glm::mat4 m_projectionMatrix{1.0f};
		glm::mat4 m_viewMatrix{1.0f};
};

} // FFL

#endif // CAMERA_HPP
