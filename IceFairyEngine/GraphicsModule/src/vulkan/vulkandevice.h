#pragma once

#include <set>

#include "vulkan/vulkan.hpp"

#include "queuefamily.h"
#include "vulkanexception.h"

namespace IceFairy {

	class VulkanDevice {
	public:
		VulkanDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices);

		const vk::UniqueDevice& GetDevice(void);
		vk::Queue GetGraphicsQueue(void);
		vk::Queue GetPresentQueue(void);

	private:
		vk::UniqueDevice CreateDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices);

		const vk::UniqueDevice device;

		QueueFamily::Indices indices;
	};

}