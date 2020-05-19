#pragma once

#include "vulkandevice.h"

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

vk::SwapchainKHR IceFairy::VulkanDevice::CreateSwapChain(SwapChainSupportDetails swapChainSupport) {
	return vk::SwapchainKHR();
	//SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	//vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	//vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	//vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	//uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	//if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
	//	imageCount = swapChainSupport.capabilities.maxImageCount;
	//}

	//vk::SwapchainCreateInfoKHR createInfo({}, surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
	//	extent, 1, vk::ImageUsageFlagBits::eColorAttachment);

	//QueueFamily::Indices indices = QueueFamily(physicalDevice, surface).FindQueueFamilies();
	//uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	//if (indices.graphicsFamily != indices.presentFamily) {
	//	createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
	//	createInfo.queueFamilyIndexCount = 2;
	//	createInfo.pQueueFamilyIndices = queueFamilyIndices;
	//}
	//else {
	//	createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	//	createInfo.queueFamilyIndexCount = 0; // Optional
	//	createInfo.pQueueFamilyIndices = nullptr; // Optional
	//}

	//createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	//createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	//createInfo.presentMode = presentMode;
	//createInfo.clipped = VK_TRUE;
	//createInfo.oldSwapchain = nullptr;

	//try {
	//	return device->GetDevice()->createSwapchainKHR(createInfo);
	//}
	//catch (std::runtime_error err) {
	//	throw VulkanException(std::string("Failed to create swap chain: ") + err.what());
	//}

	//swapChainImages = device->GetDevice()->getSwapchainImagesKHR(swapChain);

	//swapChainImageFormat = surfaceFormat.format;
	//swapChainExtent = extent;
}
