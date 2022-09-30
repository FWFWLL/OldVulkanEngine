#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Model.hpp"
#include "glm/trigonometric.hpp"

// Libraries
#include <glm/fwd.hpp>

// STD
#include <memory>
#include <sys/types.h>

namespace FFL {

struct Transform2DComponent {
	glm::vec2 translation = {};
	glm::vec2 scale = {1.0f, 1.0f};
	float rotation;

	glm::mat2 mat2() {
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);

		glm::mat2 rotationMatrix = {
			{c, s},
			{-s, c},
		};

		glm::mat2 scaleMatrix = {
			{scale.x, 0.0f},
			{0.0f, scale.y},
		};

		return rotationMatrix * scaleMatrix;
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
	Transform2DComponent transform2D = {};

	static GameObject createGameObject() {
		static id_t currentId = 0;
		return GameObject{currentId++};
	}

	id_t getId() const {return m_id;}
private:
	GameObject(id_t p_objId) : m_id{p_objId} {}

	id_t m_id;
};

}

#endif // GAMEOBJECT_HPP
