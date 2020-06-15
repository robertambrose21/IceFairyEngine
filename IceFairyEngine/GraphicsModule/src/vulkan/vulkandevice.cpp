#pragma once

#include "vulkandevice.h"

IceFairy::VulkanDevice::VulkanDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, QueueFamily::Indices indices) :
	indices(indices),
	physicalDevice(physicalDevice),
	surface(surface),
	device(CreateDevice(physicalDevice, surface, indices)) {
}

const vk::UniqueDevice& IceFairy::VulkanDevice::GetDevice(void) {
	return device;
}

vk::UniqueDevice IceFairy::VulkanDevice::CreateDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, QueueFamily::Indices indices) {
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

IceFairy::SwapChainSupportDetails::Data IceFairy::VulkanDevice::CreateSwapChain(GLFWwindow* window) {
	SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(physicalDevice, surface);

	vk::SurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
	vk::PresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
	vk::Extent2D extent = swapChainSupport.ChooseSwapExtent(window);

	uint32_t imageCount = swapChainSupport.GetCapabilities().minImageCount + 1;
	if (swapChainSupport.GetCapabilities().maxImageCount > 0 && imageCount > swapChainSupport.GetCapabilities().maxImageCount) {
		imageCount = swapChainSupport.GetCapabilities().maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo({}, surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
		extent, 1, vk::ImageUsageFlagBits::eColorAttachment);

	QueueFamily::Indices indices = QueueFamily(physicalDevice, surface).FindQueueFamilies();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.GetCapabilities().currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = swapChainSupport.ChooseSwapPresentMode();
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	try {
		auto swapChain = device->createSwapchainKHR(createInfo);

		return {
			swapChain,
			device->getSwapchainImagesKHR(swapChain),
			surfaceFormat.format,
			extent
		};
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create swap chain: ") + err.what());
	}
}

vk::DescriptorPool IceFairy::VulkanDevice::CreateDescriptorPool(const uint32_t& numSwapChainImages) {
	std::array<vk::DescriptorPoolSize, 2> poolSizes = {
		vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, numSwapChainImages),
		vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, numSwapChainImages)
	};

	vk::DescriptorPoolCreateInfo poolInfo({}, numSwapChainImages,
		static_cast<uint32_t>(poolSizes.size()), poolSizes.data());

	descriptorPool = device->createDescriptorPool(poolInfo);
	return descriptorPool;
}

vk::DescriptorSetLayout IceFairy::VulkanDevice::CreateDescriptorSetLayout(void) {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1,
		vk::ShaderStageFlagBits::eVertex);
	vk::DescriptorSetLayoutBinding samplerLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1,
		vk::ShaderStageFlagBits::eFragment);

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo({}, static_cast<uint32_t>(bindings.size()), bindings.data());

	try {
		descriptorSetLayout = device->createDescriptorSetLayout(layoutInfo);
		return descriptorSetLayout;
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create descriptor set layout!");
	}
}

std::vector<vk::DescriptorSet> IceFairy::VulkanDevice::CreateDescriptorSets(const uint32_t& numSwapChainImages,
	std::vector<std::pair<vk::Buffer, vma::Allocation>> uniformBuffers, vk::Sampler textureSampler,
	vk::ImageView textureImageView, vk::DeviceSize range) {
	std::vector<vk::DescriptorSetLayout> layouts(numSwapChainImages, descriptorSetLayout);

	vk::DescriptorSetAllocateInfo allocInfo(descriptorPool, numSwapChainImages, layouts.data());

	try {
		descriptorSets = device->allocateDescriptorSets(allocInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < numSwapChainImages; i++) {
		vk::DescriptorBufferInfo bufferInfo(uniformBuffers[i].first, 0, range);

		vk::DescriptorImageInfo imageInfo(textureSampler, textureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);

		std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {
			vk::WriteDescriptorSet(descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo),
			vk::WriteDescriptorSet(descriptorSets[i], 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr)
		};

		device->updateDescriptorSets(descriptorWrites, nullptr);
	}

	return descriptorSets;
}

vk::ImageView IceFairy::VulkanDevice::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels) {
	vk::ImageViewCreateInfo viewInfo({}, image, vk::ImageViewType::e2D, format, {}, vk::ImageSubresourceRange(aspectFlags, 0, mipLevels, 0, 1));

	try {
		return device->createImageView(viewInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create texture image view: ") + err.what());
	}
}

vk::Sampler IceFairy::VulkanDevice::CreateTextureSampler(const uint32_t& mipLevels) {
	vk::SamplerCreateInfo samplerInfo({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f, VK_TRUE,
		16.0f, VK_FALSE, vk::CompareOp::eAlways, 0, static_cast<float>(mipLevels));

	try {
		return device->createSampler(samplerInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create texture sampler!");
	}
}

void IceFairy::VulkanDevice::WaitIdle(void) {
	device->waitIdle();
}
