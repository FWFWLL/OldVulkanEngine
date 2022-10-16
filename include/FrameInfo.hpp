#ifndef FRAMEINFO_HPP
#define FRAMEINFO_HPP

#include "Camera.hpp"
#include "GameObject.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

namespace FFL {

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
