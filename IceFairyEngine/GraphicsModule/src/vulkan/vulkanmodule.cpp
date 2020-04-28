#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define VMA_IMPLEMENTATION
#include "memory-allocator/vk_mem_alloc.hpp"

#include "vulkanmodule.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::string MODEL_PATH = "models/chalet.obj";
const std::string TEXTURE_PATH = "textures/chalet.jpg";

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance, 
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
	const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

bool CheckValidationLayerSupport(void) {
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

IceFairy::VulkanModule::VulkanModule() : Module()
{ }

// TODO: Catch VulkanExceptions and do stuff sensibly with them
void IceFairy::VulkanModule::CheckPreconditions(void) {
	if (windowWidth == 0) {
		throw VulkanException("Window width has not been set (currently 0)");
	}

	if (windowHeight == 0) {
		throw VulkanException("Window height has not been set (currently 0)");
	}
}

std::string IceFairy::VulkanModule::GetName(void) const {
	return Constants::VulkanModuleName;
}

bool IceFairy::VulkanModule::Initialise(void) {
	CheckPreconditions();

	InitialiseWindow();

	InitialiseVulkanInstance();
	SetupDebugCallback();

	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateMemoryAllocator();
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	CreateGraphicsPipeline();
	commandPoolManager = std::make_shared<CommandPoolManager>(device, physicalDevice, surface);
	CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	CreateTextureImage();
	CreateTextureImageView();
	CreateTextureSampler();
	LoadModel();

	for (auto& vertexObject : vertexObjects) {
		CreateVertexBuffer(vertexObject);
		CreateIndexBuffer(vertexObject);
	}

	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	commandPoolManager->CreateCommandBuffers(
		commandBuffers,
		vertexObjects,
		swapChainFramebuffers,
		renderPass,
		swapChainExtent,
		graphicsPipeline,
		pipelineLayout,
		descriptorSets
	);
	CreateSyncObjects();

	return true;
}

// TODO: Proper clean up
// TODO: Device stuff potentially unneeded?
void IceFairy::VulkanModule::CleanUp(void) {
	CleanupSwapChain();

	device->GetDevice()->destroySampler(textureSampler, nullptr);
	device->GetDevice()->destroyImageView(textureImageView, nullptr);

	allocator.destroyImage(textureImage, textureImageMemory);

	device->GetDevice()->destroyDescriptorSetLayout(descriptorSetLayout, nullptr);

	for (auto& vertexObject : vertexObjects) {
		vertexObject.CleanUp(allocator);
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		
		device->GetDevice()->destroySemaphore(renderFinishedSemaphores[i], nullptr);
		device->GetDevice()->destroySemaphore(imageAvailableSemaphores[i], nullptr);
		device->GetDevice()->destroyFence(inFlightFences[i], nullptr);
	}

	commandPoolManager->CleanUp();

	vmaDestroyAllocator(allocator);

	device->GetDevice()->destroy();

	if (enableValidationLayers) {
		vkDestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void IceFairy::VulkanModule::StartMainLoop(void) {
	RunMainLoop();
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto vulkanModule = reinterpret_cast<IceFairy::VulkanModule*>(glfwGetWindowUserPointer(window));
	vulkanModule->SetIsFrameBufferResized(true);
}

void IceFairy::VulkanModule::InitialiseWindow(void) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void IceFairy::VulkanModule::AddVertexObject(const VertexObject& vertexObject) {
	vertexObjects.push_back(vertexObject);
}

void IceFairy::VulkanModule::SetWindowWidth(const int& windowWidth) {
	this->windowWidth = windowWidth;
}

void IceFairy::VulkanModule::SetWindowHeight(const int& windowHeight) {
	this->windowHeight = windowHeight;
}

int IceFairy::VulkanModule::GetWindowWidth(void) const {
	return windowWidth;
}

int IceFairy::VulkanModule::GetWindowHeight(void) const {
	return windowHeight;
}

GLFWwindow* IceFairy::VulkanModule::GetWindow(void) {
	return window;
}

void IceFairy::VulkanModule::SetIsFrameBufferResized(const bool& value) {
	this->isFrameBufferResized = value;
}

void IceFairy::VulkanModule::InitialiseVulkanInstance(void) {
	if (enableValidationLayers && !CheckValidationLayerSupport()) {
		throw VulkanException("validation layers requested, but not available!");
	}

	// TODO: Catch globally as a VulkanException
	try {
		uint32_t numLayers = 0;
		const char* const* layerNames = nullptr;
		auto extensions = GetRequiredExtensions();

		if (enableValidationLayers) {
			numLayers = static_cast<uint32_t>(validationLayers.size());
			layerNames = validationLayers.data();
		}

		vk::ApplicationInfo appInfo("Vulkan Demo", 0, "IceFairyEngine", 0, VK_API_VERSION_1_1);
		vk::InstanceCreateInfo createInfo({}, &appInfo, numLayers, layerNames, static_cast<uint32_t>(extensions.size()), extensions.data());

		instance = vk::createInstance(createInfo);;
	}
	catch (std::runtime_error& err) {
		throw VulkanException("failed to create instance: " + std::string(err.what()));
	}
}

void IceFairy::VulkanModule::CreateSurface(void) {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw VulkanException("failed to create window surface!");
	}
}

void IceFairy::VulkanModule::CreateSwapChain(void) {
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo({}, surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
		extent, 1, vk::ImageUsageFlagBits::eColorAttachment);

	QueueFamily::Indices indices = QueueFamily(physicalDevice, surface).FindQueueFamilies();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	try {
		swapChain = device->GetDevice()->createSwapchainKHR(createInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create swap chain: ") + err.what());
	}

	swapChainImages = device->GetDevice()->getSwapchainImagesKHR(swapChain);

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void IceFairy::VulkanModule::PickPhysicalDevice(void) {
	std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

	if (devices.empty()) {
		throw VulkanException("failed to find GPUs with Vulkan support!");
	}

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			msaaSamples = GetMaxUsableSampleCount();
			return;
		}
	}

	throw VulkanException("failed to find a suitable GPU!");
}

bool IceFairy::VulkanModule::IsDeviceSuitable(vk::PhysicalDevice device) {
	QueueFamily::Indices indices = QueueFamily(device, surface).FindQueueFamilies();

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void IceFairy::VulkanModule::CreateLogicalDevice(void) {
	// TODO: Move QueueFamily to central location so it doesn't have to be called twice, VulkanContext maybe?
	device = std::make_shared<VulkanDevice>(physicalDevice, surface, QueueFamily(physicalDevice, surface).FindQueueFamilies());
}

void IceFairy::VulkanModule::CreateMemoryAllocator(void) {
	allocator = vma::createAllocator(vma::AllocatorCreateInfo({}, physicalDevice, *device->GetDevice()));
}

void IceFairy::VulkanModule::CreateImageViews(void) {
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1);
	}
}

vk::ImageView IceFairy::VulkanModule::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels) {
	vk::ImageViewCreateInfo viewInfo({}, image, vk::ImageViewType::e2D, format, {}, vk::ImageSubresourceRange(aspectFlags, 0, mipLevels, 0, 1));

	try {
		return device->GetDevice()->createImageView(viewInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create texture image view: ") + err.what());
	}
}

// TODO: We probably want to move texture creation to another class
void IceFairy::VulkanModule::CreateTextureImage(void) {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	vk::DeviceSize imageSize = (uint64_t) texWidth * texHeight * 4;
	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels) {
		throw VulkanException("failed to load texture image!");
	}

	auto [stagingBuffer, stagingAllocation] = CreateBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible |
		vk::MemoryPropertyFlagBits::eHostCoherent);

	void* data;
	allocator.mapMemory(stagingAllocation, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	allocator.unmapMemory(stagingAllocation);
	allocator.flushAllocation(stagingAllocation, 0, imageSize);

	stbi_image_free(pixels);

	CreateImage(texWidth, texHeight, mipLevels, vk::SampleCountFlagBits::e1, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

	TransitionImageLayout(textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, mipLevels);
	CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	// Transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

	GenerateMipmaps(textureImage, vk::Format::eR8G8B8A8Unorm, texWidth, texHeight, mipLevels);

	allocator.destroyBuffer(stagingBuffer, stagingAllocation);
}

void IceFairy::VulkanModule::GenerateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
	vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(imageFormat);

	if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
		throw VulkanException("texture image format does not support linear blitting!");
	}

	vk::CommandBuffer commandBuffer = commandPoolManager->BeginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier({}, {}, {}, {}, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image,
		vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
			{},
			0, nullptr,
			0, nullptr,
			1, &barrier);

		std::array<vk::Offset3D, 2> srcOffsets = {
			{{0, 0, 0}, {mipWidth, mipHeight, 1}}
		};

		std::array<vk::Offset3D, 2> dstOffsets = {
			{{0, 0, 0}, { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }}
		};

		vk::ImageBlit blit(vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, i - 1, 0, 1), srcOffsets,
			vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, i, 0, 1), dstOffsets);

		commandBuffer.blitImage(
			image, vk::ImageLayout::eTransferSrcOptimal,
			image, vk::ImageLayout::eTransferDstOptimal,
			{ blit },
			vk::Filter::eLinear);

		barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
			{},
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
		{},
		0, nullptr,
		0, nullptr,
		1, &barrier);

	commandPoolManager->EndSingleTimeCommands(commandBuffer);
}

void IceFairy::VulkanModule::CreateTextureImageView(void) {
	textureImageView = CreateImageView(textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, mipLevels);
}

void IceFairy::VulkanModule::CreateTextureSampler(void) {
	vk::SamplerCreateInfo samplerInfo({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f, VK_TRUE,
		16.0f, VK_FALSE, vk::CompareOp::eAlways, 0, static_cast<float>(mipLevels));

	try {
		textureSampler = device->GetDevice()->createSampler(samplerInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create texture sampler!");
	}
}

vk::SampleCountFlagBits IceFairy::VulkanModule::GetMaxUsableSampleCount(void) {
	vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
	vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
	if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
	if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
	if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
	if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
	if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

	return vk::SampleCountFlagBits::e1;
}

void IceFairy::VulkanModule::CreateColorResources(void) {
	vk::Format colorFormat = swapChainImageFormat;

	CreateImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal, colorImage, colorImageMemory);
	colorImageView = CreateImageView(colorImage, colorFormat, vk::ImageAspectFlagBits::eColor, 1);
}

void IceFairy::VulkanModule::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling,
		vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vma::Allocation& imageMemory) {
	vk::ImageCreateInfo imageInfo({}, vk::ImageType::e2D, format, vk::Extent3D(width, height, 1), mipLevels, 1,
		numSamples, tiling, usage, vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined);

	try {
		image = device->GetDevice()->createImage(imageInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to create image: ") + err.what());
	}

	vk::MemoryRequirements memRequirements = device->GetDevice()->getImageMemoryRequirements(image);

	try {
		imageMemory = allocator.allocateMemory(memRequirements, vma::AllocationCreateInfo({}, vma::MemoryUsage::eGpuOnly, properties));
	}
	catch (std::runtime_error err) {
		throw VulkanException(std::string("Failed to allocate image memory: ") + err.what());
	}

	allocator.bindImageMemory(imageMemory, image);
}

// TODO: Find out what the fuck is going on in this method
void IceFairy::VulkanModule::TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels) {
	vk::CommandBuffer commandBuffer = commandPoolManager->BeginSingleTimeCommands();

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;
	vk::AccessFlags srcAccessMask, dstAccessMask;
	vk::ImageAspectFlags subResourceRangeAspectMask;

	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		subResourceRangeAspectMask = vk::ImageAspectFlagBits::eDepth;

		if (HasStencilComponent(format)) {
			subResourceRangeAspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}
	else {
		subResourceRangeAspectMask = vk::ImageAspectFlagBits::eColor;
	}

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
		dstAccessMask = vk::AccessFlagBits::eTransferWrite;
		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}
	else {
		throw VulkanException("unsupported layout transition!");
	}

	vk::ImageSubresourceRange subResourceRange(subResourceRangeAspectMask, 0, mipLevels, 0, 1);

	vk::ImageMemoryBarrier barrier(srcAccessMask, dstAccessMask, oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED, image, subResourceRange);

	commandBuffer.pipelineBarrier(
		sourceStage, destinationStage,
		{},
		0, nullptr,
		0, nullptr,
		1, &barrier);

	commandPoolManager->EndSingleTimeCommands(commandBuffer);
}

void IceFairy::VulkanModule::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height) {
	vk::CommandBuffer commandBuffer = commandPoolManager->BeginSingleTimeCommands();

	vk::BufferImageCopy region(0, 0, 0, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
		{ 0, 0, 0 }, { width, height, 1 });

	commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, { region });

	commandPoolManager->EndSingleTimeCommands(commandBuffer);
}

void IceFairy::VulkanModule::CreateDescriptorSetLayout(void) {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1,
		vk::ShaderStageFlagBits::eVertex);
	vk::DescriptorSetLayoutBinding samplerLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1,
		vk::ShaderStageFlagBits::eFragment);

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo({}, static_cast<uint32_t>(bindings.size()), bindings.data());

	try {
		descriptorSetLayout = device->GetDevice()->createDescriptorSetLayout(layoutInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create descriptor set layout!");
	}
}

void IceFairy::VulkanModule::CreateUniformBuffers(void) {
	vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		uniformBuffers[i] = CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible
			| vk::MemoryPropertyFlagBits::eHostCoherent);
	}
}

// Binds shaders and position/colour/texture coords
void IceFairy::VulkanModule::CreateGraphicsPipeline(void) {
	ShaderModule module(*device->GetDevice());

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

	vk::Viewport viewport(0.0f, 0.0f, (float)swapChainExtent.width, (float)swapChainExtent.height, 0.0f, 1.0f);

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

	// TODO: Catch globally
	try {
		pipelineLayout = device->GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo({}, 1, &descriptorSetLayout));
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create pipeline layout!");
	}

	vk::GraphicsPipelineCreateInfo pipelineInfo({}, 2, shaderStages, &vertexInputInfo, &inputAssembly, nullptr,
		&viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, pipelineLayout, renderPass, 0, {}, -1);

	// TODO: Use "createGraphicsPipelines" for multiple pipelines
	// TODO: Catch globally
	try {
		graphicsPipeline = device->GetDevice()->createGraphicsPipeline({}, pipelineInfo);
		module.CleanUp();
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create graphics pipeline!");
	}
}

void IceFairy::VulkanModule::CreateRenderPass(void) {
	vk::AttachmentDescription attachments[3];

	attachments[0] = vk::AttachmentDescription({}, swapChainImageFormat, msaaSamples, vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal);

	attachments[1] = vk::AttachmentDescription({}, FindDepthFormat(), msaaSamples, vk::AttachmentLoadOp::eClear,
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

	// TODO: Catch globally
	try {
		renderPass = device->GetDevice()->createRenderPass(vk::RenderPassCreateInfo({}, 3, attachments, 1, &subpass, 1, &dependency));
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create render pass!");
	}
}

void IceFairy::VulkanModule::CreateFrameBuffers(void) {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::array<vk::ImageView, 3> attachments = {
			colorImageView,
			depthImageView,
			swapChainImageViews[i]
		};

		vk::FramebufferCreateInfo framebufferInfo({}, renderPass, static_cast<uint32_t>(attachments.size()),
			attachments.data(), swapChainExtent.width, swapChainExtent.height, 1);

		// TODO: Sort this out
		try {
			swapChainFramebuffers[i] = device->GetDevice()->createFramebuffer(framebufferInfo);
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to create framebuffer!");
		}
	}
}

std::pair<vk::Buffer, vma::Allocation> IceFairy::VulkanModule::CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
	vk::MemoryPropertyFlags properties)
{
	vk::BufferCreateInfo bufferInfo({}, size, usage, vk::SharingMode::eExclusive);

	vma::AllocationCreateInfo allocInfo({}, vma::MemoryUsage::eGpuOnly, properties);

	// TODO: Sort out
	try {
		return allocator.createBuffer(bufferInfo, allocInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to allocate buffer memory!");
	}
}

void IceFairy::VulkanModule::CopyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
	vk::CommandBuffer commandBuffer = commandPoolManager->BeginSingleTimeCommands();

	commandBuffer.copyBuffer(srcBuffer, dstBuffer, { vk::BufferCopy(0, 0, size) });

	commandPoolManager->EndSingleTimeCommands(commandBuffer);
}

// TODO: Eventually combine vertex and index buffers into same buffer - see if we can combine multiple buffers too?
void IceFairy::VulkanModule::CreateVertexBuffer(VertexObject& vertexObject) {
	vk::DeviceSize bufferSize = sizeof(vertexObject.GetVertices()[0]) * vertexObject.GetVertices().size();

	auto [stagingBuffer, stagingAllocation] = CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

	void* data;
	allocator.mapMemory(stagingAllocation, &data);
	memcpy(data, vertexObject.GetVertices().data(), (size_t)bufferSize);
	allocator.unmapMemory(stagingAllocation);
	// TODO: Apparently we have to do this? Doesn't seem to make a difference - find out why. Check VMA docs
	// v
	// We went for the first approach, which ensures that the mapped memory always matches the contents of the allocated memory.
	// Do keep in mind that this may lead to slightly worse performance than explicit flushing, but we'll see why that doesn't matter in the next chapter.
	// https://vulkan-tutorial.com/Vertex_buffers/Vertex_buffer_creation
	allocator.flushAllocation(stagingAllocation, 0, bufferSize);

	auto& bufferAllocation = CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst |
		vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);

	vertexObject.AssignVertexBuffer(bufferAllocation);

	CopyBuffer(stagingBuffer, vertexObject.GetVertexBuffer(), bufferSize);

	allocator.destroyBuffer(stagingBuffer, stagingAllocation);
}

void IceFairy::VulkanModule::CreateIndexBuffer(VertexObject& vertexObject) {
	vk::DeviceSize bufferSize = sizeof(vertexObject.GetIndices()[0]) * vertexObject.GetIndices().size();

	auto [stagingBuffer, stagingAllocation] = CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible |
		vk::MemoryPropertyFlagBits::eHostCoherent);

	void* data;
	allocator.mapMemory(stagingAllocation, &data);
	memcpy(data, vertexObject.GetIndices().data(), (size_t)bufferSize);
	allocator.unmapMemory(stagingAllocation);
	allocator.flushAllocation(stagingAllocation, 0, bufferSize);

	auto& bufferAllocation = CreateBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal);

	vertexObject.AssignIndexBuffer(bufferAllocation);

	CopyBuffer(stagingBuffer, vertexObject.GetIndexBuffer(), bufferSize);

	allocator.destroyBuffer(stagingBuffer, stagingAllocation);
}

// TODO: Do we need this anymore? VMA seems to handle it. Might want to look into the docs and vulkan tutorial to find out
// exactly what's going on here.
uint32_t IceFairy::VulkanModule::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw VulkanException("failed to find suitable memory type!");

}

// TODO: Temporarily disabled
void IceFairy::VulkanModule::LoadModel(void) {
	/*tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
		throw VulkanException(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}*/
}

void IceFairy::VulkanModule::CreateSyncObjects(void) {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	vk::SemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.flags = vk::SemaphoreCreateFlagBits();

	vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (device->GetDevice()->createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != vk::Result::eSuccess ||
			device->GetDevice()->createSemaphore(&semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != vk::Result::eSuccess) {

			throw VulkanException("failed to create synchronization objects for a frame!");
		}

		try {
			inFlightFences[i] = device->GetDevice()->createFence(fenceInfo);
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to create synchronization objects for a frame!");
		}
	}
}

std::vector<const char*> IceFairy::VulkanModule::GetRequiredExtensions(void) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool IceFairy::VulkanModule::CheckDeviceExtensionSupport(vk::PhysicalDevice device) {
	std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

IceFairy::VulkanModule::SwapChainSupportDetails IceFairy::VulkanModule::QuerySwapChainSupport(vk::PhysicalDevice device) {
	// TODO: Constructor for this
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}

vk::SurfaceFormatKHR IceFairy::VulkanModule::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

vk::PresentModeKHR IceFairy::VulkanModule::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes) {
	vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
		else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

vk::Extent2D IceFairy::VulkanModule::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR & capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	vk::Extent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};

	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

	return actualExtent;
}

void IceFairy::VulkanModule::CleanupSwapChain(void) {
	device->GetDevice()->destroyImageView(colorImageView, nullptr);
	allocator.destroyImage(colorImage, colorImageMemory);

	device->GetDevice()->destroyImageView(depthImageView, nullptr);
	allocator.destroyImage(depthImage, depthImageMemory);

	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		device->GetDevice()->destroyFramebuffer(swapChainFramebuffers[i], nullptr);
	}

	commandPoolManager->FreeCommandBuffers(commandBuffers);

	device->GetDevice()->destroyPipeline(graphicsPipeline, nullptr);
	device->GetDevice()->destroyPipelineLayout(pipelineLayout, nullptr);
	device->GetDevice()->destroyRenderPass(renderPass, nullptr);

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		device->GetDevice()->destroyImageView(swapChainImageViews[i], nullptr);
	}

	device->GetDevice()->destroySwapchainKHR(swapChain, nullptr);

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		allocator.destroyBuffer(uniformBuffers[i].first, uniformBuffers[i].second);
	}

	device->GetDevice()->destroyDescriptorPool(descriptorPool, nullptr);
}

void IceFairy::VulkanModule::RecreateSwapChain(void) {
	// TODO: This while is when the window is minimized - wait until we unminimize. Separate function might be nice
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	device->GetDevice()->waitIdle();

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();

	commandPoolManager->CreateCommandBuffers(
		commandBuffers,
		vertexObjects,
		swapChainFramebuffers,
		renderPass,
		swapChainExtent,
		graphicsPipeline,
		pipelineLayout,
		descriptorSets
	);
}

void IceFairy::VulkanModule::RunMainLoop(void) {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		DrawFrame();
	}

	device->GetDevice()->waitIdle();
}

void IceFairy::VulkanModule::DrawFrame(void) {
	device->GetDevice()->waitForFences({ inFlightFences[currentFrame] }, VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	vk::Result result = device->GetDevice()->acquireNextImageKHR(swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], nullptr, &imageIndex);

	if (result == vk::Result::eErrorOutOfDateKHR) {
		RecreateSwapChain();
		return;
	}
	else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
		throw VulkanException("failed to acquire swap chain image!");
	}

	UpdateUniformBuffer(imageIndex);

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &commandBuffers[imageIndex], 1, signalSemaphores);

	device->GetDevice()->resetFences({ inFlightFences[currentFrame] });

	try {
		device->GetGraphicsQueue().submit({ submitInfo }, inFlightFences[currentFrame]);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to submit draw command buffer!");
	}

	vk::SwapchainKHR swapChains[] = { swapChain };

	result = device->GetPresentQueue().presentKHR(vk::PresentInfoKHR(1, signalSemaphores, 1, swapChains, &imageIndex, nullptr));

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || isFrameBufferResized) {
		isFrameBufferResized = false;
		RecreateSwapChain();
	}
	else if (result != vk::Result::eSuccess) {
		throw VulkanException("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void IceFairy::VulkanModule::UpdateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	// TODO: This is the camera, need to create a separate class for it
	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

	/*GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.The easiest way to
	compensate for that is to flip the sign on the scaling factor of the Y axis in the projection matrix.If you don't do
	this, then the image will be rendered upside down.*/
	ubo.proj[1][1] *= -1;

	void* data;
	allocator.mapMemory(uniformBuffers[currentImage].second, &data);
	memcpy(data, &ubo, sizeof(ubo));
	allocator.unmapMemory(uniformBuffers[currentImage].second);
}

void IceFairy::VulkanModule::CreateDescriptorPool(void) {
	std::array<vk::DescriptorPoolSize, 2> poolSizes = {
		vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(swapChainImages.size())),
		vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(swapChainImages.size()))
	};

	vk::DescriptorPoolCreateInfo poolInfo({}, static_cast<uint32_t>(swapChainImages.size()),
		static_cast<uint32_t>(poolSizes.size()), poolSizes.data());

	descriptorPool = device->GetDevice()->createDescriptorPool(poolInfo);
}

// TODO: Move these into something else, material construction
void IceFairy::VulkanModule::CreateDescriptorSets(void) {
	std::vector<vk::DescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);

	vk::DescriptorSetAllocateInfo allocInfo(descriptorPool, static_cast<uint32_t>(swapChainImages.size()), layouts.data());

	try {
		descriptorSets = device->GetDevice()->allocateDescriptorSets(allocInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		vk::DescriptorBufferInfo bufferInfo(uniformBuffers[i].first, 0, sizeof(UniformBufferObject));

		vk::DescriptorImageInfo imageInfo(textureSampler, textureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);

		std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {
			vk::WriteDescriptorSet(descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo),
			vk::WriteDescriptorSet(descriptorSets[i], 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr)
		};

		device->GetDevice()->updateDescriptorSets(descriptorWrites, nullptr);
	}
}

void IceFairy::VulkanModule::CreateDepthResources(void) {
	vk::Format depthFormat = FindDepthFormat();

	CreateImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
	depthImageView = CreateImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);
	TransitionImageLayout(depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1);
}

vk::Format IceFairy::VulkanModule::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
	for (vk::Format format : candidates) {
		vk::FormatProperties props = physicalDevice.getFormatProperties(format);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw VulkanException("failed to find supported format!");
}

vk::Format IceFairy::VulkanModule::FindDepthFormat(void) {
	return FindSupportedFormat(
		{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}

bool IceFairy::VulkanModule::HasStencilComponent(vk::Format format) {
	return format == vk::Format::eD32SfloatS8Uint|| format == vk::Format::eD24UnormS8Uint;
}

void IceFairy::VulkanModule::SetupDebugCallback(void) {
	if (!enableValidationLayers)
		return;

	try {
		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

		callback = instance.createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, debugCallback));
	}
	catch (std::runtime_error& err) {
		throw VulkanException("failed to set up debug callback: " + std::string(err.what()));
	}

	if (Logger::GetLogLevel() != Logger::LEVEL_TRACE) {
		Logger::PrintLn(Logger::LEVEL_DEBUG,
			"Note that to view 'Verbose' Vulkan validation layer messages (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT flag); a debug level of TRACE must be used.");
	}

}
