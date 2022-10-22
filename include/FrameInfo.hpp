#ifndef FRAMEINFO_HPP
#define FRAMEINFO_HPP

#include "Camera.hpp"
#include "GameObject.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

namespace FFL {

#define MAX_LIGHTS 10

struct PointLight {
	glm::vec4 position = {};
	glm::vec4 color = {};
};

struct GlobalUniformBufferObject {
	glm::mat4 projection{1.0f};
	glm::mat4 view{1.0f};
	glm::mat4 inverseView{1.0f};
	glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f};
	PointLight pointLights[MAX_LIGHTS];
	int numLights;
};

struct FrameInfo {
	int frameIndex;
	float deltaTime;
	VkCommandBuffer commandBuffer;
	Camera& camera;
	VkDescriptorSet globalDescriptorSet;
	GameObject::Map& gameObjects;
};

} // FFL

#endif // FRAMEINFO_HPP
