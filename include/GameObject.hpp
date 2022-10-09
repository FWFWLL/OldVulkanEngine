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

	glm::mat4 mat4();
	glm::mat3 normalMatrix();
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
