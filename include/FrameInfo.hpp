#ifndef FRAMEINFO_HPP
#define FRAMEINFO_HPP

#include "Camera.hpp"

// Libraries
#include <vulkan/vulkan_core.h>

namespace FFL {

struct FrameInfo {
	int frameIndex;
	float deltaTime;
	VkCommandBuffer commandBuffer;
	Camera& camera;
};

} // FFL

#endif // FRAMEINFO_HPP
