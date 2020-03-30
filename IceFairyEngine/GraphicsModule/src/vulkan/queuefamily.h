#pragma once

#include "vulkan/vulkan.hpp"

#include <optional>

namespace IceFairy {

	class QueueFamily {
	public:
		typedef struct {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		} Indices;

		QueueFamily(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);

		Indices FindQueueFamilies(void);

	private:
		vk::PhysicalDevice physicalDevice;
		VkSurfaceKHR surface;
	};

}
