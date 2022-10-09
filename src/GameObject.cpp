#include "GameObject.hpp"

namespace FFL {

glm::mat4 TransformComponent::mat4() {
	// Matrix corresponds to translate * R_y * R_x * R_z * scale transformation
	// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
	// Conversion to a rotation matrix using this formula:
	// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	const float s1 = glm::sin(rotation.y);
	const float c1 = glm::cos(rotation.y);
	const float s2 = glm::sin(rotation.x);
	const float c2 = glm::cos(rotation.x);
	const float s3 = glm::sin(rotation.z);
	const float c3 = glm::cos(rotation.z);

	return glm::mat4 {
		{
			scale.x * (c1 * c3 + s1* s2 * s3),
			scale.x * (c2 * s3),
			scale.x * (c1* s2 * s3 - c3 * s1),
			0.0f,
		},
		{
			scale.y * (c3* s1 * s2 - c1 * s3),
			scale.y * (c2 * c3),
			scale.y * (c1* c3 * s2 + s1 * s3),
			0.0f,
		},
		{
			scale.z * (c2 * s1),
			scale.z * -s2,
			scale.z * (c1 * c2),
			0.0f,
		},
		{translation.x, translation.y, translation.z, 1.0f},
	};
}

glm::mat3 TransformComponent::normalMatrix() {
	const float s1 = glm::sin(rotation.y);
	const float c1 = glm::cos(rotation.y);
	const float s2 = glm::sin(rotation.x);
	const float c2 = glm::cos(rotation.x);
	const float s3 = glm::sin(rotation.z);
	const float c3 = glm::cos(rotation.z);

	const glm::vec3 invScale = 1.0f / scale;

	return glm::mat3 {
		{
			invScale.x * (c1 * c3 + s1* s2 * s3),
			invScale.x * (c2 * s3),
			invScale.x * (c1* s2 * s3 - c3 * s1),
		},
		{
			invScale.y * (c3* s1 * s2 - c1 * s3),
			invScale.y * (c2 * c3),
			invScale.y * (c1* c3 * s2 + s1 * s3),
		},
		{
			invScale.z * (c2 * s1),
			invScale.z * -s2,
			invScale.z * (c1 * c2),
		},
	};
}

}
