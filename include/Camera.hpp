#ifndef CAMERA_HPP
#define CAMERA_HPP

// Libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace FFL {

class Camera {
	public:
		void setOrthographicProjection(float p_left, float p_right, float p_top, float p_bottom, float p_near, float p_far);
		void setPerspectiveProjection(float p_fovY, float p_aspect, float p_near, float p_far);

		const glm::mat4& getProjection() const {return m_projectionMatrix;}
	private:
		glm::mat4 m_projectionMatrix{1.0f};
};

} // FFL

#endif // CAMERA_HPP
