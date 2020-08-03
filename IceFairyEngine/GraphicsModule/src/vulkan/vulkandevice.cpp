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

void IceFairy::VulkanDevice::CreateSwapChain(GLFWwindow* window) {
	SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(physicalDevice, surface);

	vk::SurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
	vk::PresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
	swapChainExtent = swapChainSupport.ChooseSwapExtent(window);

	uint32_t imageCount = swapChainSupport.GetCapabilities().minImageCount + 1;
	if (swapChainSupport.GetCapabilities().maxImageCount > 0 && imageCount > swapChainSupport.GetCapabilities().maxImageCount) {
		imageCount = swapChainSupport.GetCapabilities().maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo({}, surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
		swapChainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment);

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
		swapChain = device->createSwapchainKHR(createInfo);
		swapChainImages = device->getSwapchainImagesKHR(swapChain);
		swapChainImageFormat = surfaceFormat.format;
		swapChainImageViews = CreateImageViews();
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create swap chain: ") + err.what());
	}
}

void IceFairy::VulkanDevice::RecreateSwapChain(GLFWwindow* window) {
	CreateSwapChain(window);
	swapChainImageViews = CreateImageViews();
}

void IceFairy::VulkanDevice::CleanupSwapChain(void) {
	for (auto swapChainImageView : swapChainImageViews) {
		device->destroyImageView(swapChainImageView, nullptr);
	}

	device->destroySwapchainKHR(swapChain, nullptr);
}


std::vector<vk::ImageView> IceFairy::VulkanDevice::CreateImageViews(void) {
	std::vector<vk::ImageView> imageViews;

	for (auto swapChainImage : swapChainImages) {
		// TODO: Check the mipLevels being 1 at this point
		imageViews.push_back(CreateImageView(swapChainImage, swapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1));
	}

	return imageViews;
}

vk::DescriptorPool IceFairy::VulkanDevice::CreateDescriptorPool(void) {
	uint32_t numSwapChainImages = GetNumSwapChainImages();

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

std::vector<vk::DescriptorSet> IceFairy::VulkanDevice::CreateDescriptorSets(
	std::vector<std::pair<vk::Buffer, vma::Allocation>> uniformBuffers,
	vk::Sampler textureSampler,
	vk::ImageView textureImageView,
	vk::DeviceSize range
) {
	uint32_t numSwapChainImages = GetNumSwapChainImages();
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

vk::ImageView IceFairy::VulkanDevice::CreateImageView(
	vk::Image image,
	vk::Format format,
	vk::ImageAspectFlags aspectFlags,
	uint32_t mipLevels
) {
	vk::ImageViewCreateInfo viewInfo(
		{},
		image,
		vk::ImageViewType::e2D,
		format, {},
		vk::ImageSubresourceRange(aspectFlags, 0, mipLevels, 0, 1)
	);

	try {
		return device->createImageView(viewInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create texture image view: ") + err.what());
	}
}

vk::Sampler IceFairy::VulkanDevice::CreateTextureSampler(const uint32_t& mipLevels) {
	vk::SamplerCreateInfo samplerInfo(
		{},
		vk::Filter::eLinear,
		vk::Filter::eLinear,
		vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		0.0f,
		VK_TRUE,
		16.0f,
		VK_FALSE,
		vk::CompareOp::eAlways,
		0,
		static_cast<float>(mipLevels)
	);

	try {
		return device->createSampler(samplerInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create texture sampler!");
	}
}

std::pair<vk::Image, vma::Allocation> IceFairy::VulkanDevice::CreateImage(
	uint32_t width,
	uint32_t height,
	uint32_t mipLevels,
	vk::SampleCountFlagBits numSamples,
	vk::Format format,
	vk::ImageTiling tiling,
	vk::ImageUsageFlags usage,
	vk::MemoryPropertyFlags properties,
	vma::Allocator allocator
) {
	vk::ImageCreateInfo imageInfo(
		{},
		vk::ImageType::e2D,
		format,
		vk::Extent3D(width, height, 1),
		mipLevels,
		1,
		numSamples,
		tiling,
		usage,
		vk::SharingMode::eExclusive,
		0,
		nullptr,
		vk::ImageLayout::eUndefined
	);

	vk::Image image;
	vma::Allocation imageMemory;

	try {
		image = device->createImage(imageInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create image: ") + err.what());
	}

	vk::MemoryRequirements memRequirements = device->getImageMemoryRequirements(image);

	try {
		imageMemory = allocator.allocateMemory(memRequirements, vma::AllocationCreateInfo({}, vma::MemoryUsage::eGpuOnly, properties));
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to allocate image memory: ") + err.what());
	}

	allocator.bindImageMemory(imageMemory, image);

	return { image, imageMemory };
}

std::pair<vk::PipelineLayout, vk::Pipeline> IceFairy::VulkanDevice::CreateGraphicsPipeline(
	vk::Extent2D swapChainExtent,
	vk::SampleCountFlagBits msaaSamples,
	vk::RenderPass renderPass
) {
	ShaderModule module(*device);
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline graphicsPipeline;

	// TODO: Move elsewhere
	module.LoadFromFile("shaders/vert.spv", "shaders/frag.spv");
	vk::PipelineShaderStageCreateInfo shaderStages[] = {
		module.GetVertexShaderStageInfo(),
		module.GetFragmentShaderStageInfo()
	};

	// TODO: Move elsewhere
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo({}, 1, &bindingDescription,
		static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly({}, vk::PrimitiveTopology::eTriangleList, VK_FALSE);

	vk::Viewport viewport(0.0f, 0.0f, (float) swapChainExtent.width, (float) swapChainExtent.height, 0.0f, 1.0f);

	vk::Rect2D scissor({ 0, 0 }, swapChainExtent);

	vk::PipelineViewportStateCreateInfo viewportState({}, 1, &viewport, 1, &scissor);

	vk::PipelineRasterizationStateCreateInfo rasterizer({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill,
		vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);

	vk::PipelineMultisampleStateCreateInfo multisampling({}, msaaSamples, VK_TRUE, .2f);

	vk::PipelineDepthStencilStateCreateInfo depthStencil({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_FALSE,
		VK_FALSE, {}, {}, 0.0f, 1.0f);

	vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;

	vk::PipelineColorBlendStateCreateInfo colorBlending({}, VK_FALSE, vk::LogicOp::eCopy, 1,
		&colorBlendAttachment, { 0.0f, 0.0f, 0.0f, 0.0f });

	try {
		pipelineLayout = device->createPipelineLayout(vk::PipelineLayoutCreateInfo({}, 1, &descriptorSetLayout));
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create pipeline layout!");
	}

	vk::GraphicsPipelineCreateInfo pipelineInfo({}, 2, shaderStages, &vertexInputInfo, &inputAssembly, nullptr,
		&viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, pipelineLayout, renderPass, 0, {}, -1);

	// TODO: Use "createGraphicsPipelines" for multiple pipelines
	try {
		graphicsPipeline = device->createGraphicsPipeline({}, pipelineInfo);
		module.CleanUp();
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create graphics pipeline!");
	}

	return { pipelineLayout, graphicsPipeline };
}

vk::RenderPass IceFairy::VulkanDevice::CreateRenderPass(
	vk::Format depthFormat,
	vk::Format swapChainImageFormat,
	vk::SampleCountFlagBits msaaSamples
) {
	vk::AttachmentDescription attachments[3];

	attachments[0] = vk::AttachmentDescription({}, swapChainImageFormat, msaaSamples, vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal);

	attachments[1] = vk::AttachmentDescription({}, depthFormat, msaaSamples, vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthStencilAttachmentOptimal);

	attachments[2] = vk::AttachmentDescription({}, swapChainImageFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR);

	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	vk::AttachmentReference colorAttachmentResolveRef(2, vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1,
		&colorAttachmentRef, &colorAttachmentResolveRef, &depthAttachmentRef);

	vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eColorAttachmentOutput, {},
		vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

	try {
		return device->createRenderPass(vk::RenderPassCreateInfo({}, 3, attachments, 1, &subpass, 1, &dependency));
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create render pass!");
	}
}

std::vector<vk::Framebuffer> IceFairy::VulkanDevice::CreateFrameBuffers(
	vk::ImageView colorImageView,
	vk::ImageView depthImageView,
	vk::RenderPass renderPass
) {
	std::vector<vk::Framebuffer> swapChainFramebuffers;

	for (auto& swapChainImageView : swapChainImageViews) {
		std::array<vk::ImageView, 3> attachments = {
			colorImageView,
			depthImageView,
			swapChainImageView
		};

		vk::FramebufferCreateInfo framebufferInfo({}, renderPass, static_cast<uint32_t>(attachments.size()),
			attachments.data(), swapChainExtent.width, swapChainExtent.height, 1);

		try {
			swapChainFramebuffers.push_back(device->createFramebuffer(framebufferInfo));
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to create framebuffer!");
		}
	}

	return swapChainFramebuffers;
}

std::vector<vk::Fence> IceFairy::VulkanDevice::CreateSyncObjects(
	std::vector<vk::Semaphore>& imageAvailableSemaphores,
	std::vector<vk::Semaphore>& renderFinishedSemaphores,
	const int& maxFramesInFlight
) {
	imageAvailableSemaphores.resize(maxFramesInFlight);
	renderFinishedSemaphores.resize(maxFramesInFlight);
	std::vector<vk::Fence> inFlightFences(maxFramesInFlight);

	vk::SemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.flags = vk::SemaphoreCreateFlagBits();

	vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);

	for (size_t i = 0; i < maxFramesInFlight; i++) {
		if (device->createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != vk::Result::eSuccess ||
			device->createSemaphore(&semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != vk::Result::eSuccess) {

			throw VulkanException("failed to create synchronization objects for a frame!");
		}

		try {
			inFlightFences[i] = device->createFence(fenceInfo);
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to create synchronization objects for a frame!");
		}
	}

	return inFlightFences;
}

void IceFairy::VulkanDevice::WaitIdle(void) {
	device->waitIdle();
}

void IceFairy::VulkanDevice::WaitIdleGraphicsQueue(void) {
	device->getQueue(indices.graphicsFamily.value(), 0).waitIdle();
}

vk::Extent2D IceFairy::VulkanDevice::GetSwapChainExtent(void) const {
	return swapChainExtent;
}

vk::Format IceFairy::VulkanDevice::GetSwapChainFormat(void) const {
	return swapChainImageFormat;
}

vk::Result IceFairy::VulkanDevice::AcquireNextSwapChainImage(
	const uint32_t& timeout,
	vk::Semaphore semaphore,
	vk::Fence fence,
	uint32_t* pImageIndex
) {
	return device->acquireNextImageKHR(swapChain, timeout, semaphore, fence, pImageIndex);
}

vk::Result IceFairy::VulkanDevice::QueueImageForPresentation(
	std::vector<vk::Semaphore>& signalSemaphores,
	std::vector<uint32_t>& imageIndicies
) {
	vk::SwapchainKHR swapChains[] = { swapChain };
	vk::PresentInfoKHR presentInfo(
		signalSemaphores.size(),
		signalSemaphores.data(),
		1,
		swapChains,
		imageIndicies.data(),
		nullptr
	);

	return device->getQueue(indices.presentFamily.value(), 0).presentKHR(presentInfo);
}

void IceFairy::VulkanDevice::Submit(
	std::vector<vk::Semaphore>& waitSemaphores,
	std::vector<vk::PipelineStageFlags>& waitStages,
	std::vector<vk::Semaphore>& signalSemaphores,
	std::vector<vk::CommandBuffer>& commandBuffers,
	vk::Fence fence
) {
	vk::SubmitInfo submitInfo(
		waitSemaphores.size(),
		waitSemaphores.data(),
		waitStages.data(),
		commandBuffers.size(),
		commandBuffers.data(),
		signalSemaphores.size(),
		signalSemaphores.data()
	);

	try {
		device->getQueue(indices.graphicsFamily.value(), 0).submit({ submitInfo }, fence);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to submit draw command buffer!");
	}
}

void IceFairy::VulkanDevice::Submit(std::vector<vk::CommandBuffer>& commandBuffers) {
	Submit(
		std::vector<vk::Semaphore>{},
		std::vector<vk::PipelineStageFlags>{},
		std::vector<vk::Semaphore>{},
		commandBuffers,
		nullptr
	);
}

uint32_t IceFairy::VulkanDevice::GetNumSwapChainImages(void) const {
	return static_cast<uint32_t>(swapChainImages.size());
}
