#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Model.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

// Libraries
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

// STD
#include <memory>
#include <sys/types.h>

namespace FFL {

struct TransformComponent {
	glm::vec3 translation = {};
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};
	glm::vec3 rotation = {};

	// Matrix corresponds to translate * R_y * R_x * R_z * scale transformation
	// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
	// Conversion to a rotation matrix using this formula:
	// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	glm::mat4 mat4() {
		const float s1 = glm::sin(rotation.y);
		const float c1 = glm::cos(rotation.y);
		const float s2 = glm::sin(rotation.x);
		const float c2 = glm::cos(rotation.x);
		const float s3 = glm::sin(rotation.z);
		const float c3 = glm::cos(rotation.z);
		return glm::mat4 {
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
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
};

class GameObject {
	public:
		using id_t = unsigned int;

		// Delete copy-constructor
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		// Use default move-constructor
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		std::shared_ptr<Model> model = {};
		glm::vec3 color = {};
		TransformComponent transform = {};

		static GameObject createGameObject() {
			static id_t currentId = 0;
			return GameObject{currentId++};
		}

		id_t getId() const {return m_id;}
	private:
		GameObject(id_t p_objId) : m_id{p_objId} {}

		id_t m_id;
};

} // FFL

#endif // GAMEOBJECT_HPP
