#pragma once

#include "vulkandevice.h"
#include <iostream>

// TODO: Context object
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
// --

IceFairy::VulkanDevice::VulkanDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices) :
	indices(indices),
	device(CreateDevice(physicalDevice, surface, indices))
{ }

const vk::UniqueDevice& IceFairy::VulkanDevice::GetDevice(void) {
	return device;
}

vk::UniqueDevice IceFairy::VulkanDevice::CreateDevice(const vk::PhysicalDevice& physicalDevice, VkSurfaceKHR surface, QueueFamily::Indices indices) {
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		queueCreateInfos.push_back(vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority));
	}

	vk::PhysicalDeviceFeatures deviceFeatures;
	deviceFeatures.samplerAnisotropy = true;
	deviceFeatures.sampleRateShading = true;

	uint32_t enabledLayerCount = 0;
	const char* const* enabledLayerNames = nullptr;

	if (enableValidationLayers) {
		enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		enabledLayerNames = validationLayers.data();
	}

	vk::DeviceCreateInfo createInfo({}, static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(), enabledLayerCount,
		enabledLayerNames, static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(), &deviceFeatures);

	try {
		return physicalDevice.createDeviceUnique(createInfo);;
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create logical device: ") + err.what());
	}
}

vk::Queue IceFairy::VulkanDevice::GetGraphicsQueue(void) {
	return device->getQueue(indices.graphicsFamily.value(), 0);
}

vk::Queue IceFairy::VulkanDevice::GetPresentQueue(void) {
	return device->getQueue(indices.presentFamily.value(), 0);
}
