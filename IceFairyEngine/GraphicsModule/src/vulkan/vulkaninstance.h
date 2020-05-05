#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "vulkan/vulkan.hpp"

#include "vulkanexception.h"

namespace IceFairy {

	class VulkanInstance {
	public:
		VulkanInstance();

		const vk::UniqueInstance& GetInstance(void);

	private:
		vk::UniqueInstance CreateInstance(void);

		std::vector<const char*> GetRequiredExtensions(void);
		bool CheckValidationLayerSupport(void);

		const vk::UniqueInstance instance;
	};

}
