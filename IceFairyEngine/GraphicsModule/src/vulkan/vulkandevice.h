#pragma once

#include <set>

#include "vulkan/vulkan.hpp"

#include "swapchainsupportdetails.h"
#include "validationlayers.h"
#include "queuefamily.h"
#include "vulkanexception.h"

namespace IceFairy {

	class VulkanDevice {
	public:
		VulkanDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices);

		const vk::UniqueDevice& GetDevice(void);
		vk::Queue GetGraphicsQueue(void);
		vk::Queue GetPresentQueue(void);

		// TODO: Fix!
		vk::SwapchainKHR CreateSwapChain(SwapChainSupportDetails swapChainSupport);

	private:

		vk::UniqueDevice CreateDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices);

		const vk::UniqueDevice device;

		QueueFamily::Indices indices;
	};

}