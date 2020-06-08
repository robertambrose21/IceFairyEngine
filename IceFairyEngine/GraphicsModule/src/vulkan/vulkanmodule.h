#pragma once

#include "glfw3_include.h"
#include "glm_include.h"

#include "memory-allocator/vk_mem_alloc.hpp"
#include "vulkan/vulkan.hpp"

#include <string>
#include <vector>
#include <optional> // TODO: Remove
#include <set>
#include <algorithm>
#include <array>
#include <chrono>
#include <unordered_map>
// TODO: Offload to a resource class

#include "core/module.h"
#include "vulkanexception.h"
#include "swapchainsupportdetails.h"
#include "vulkandevice.h"
#include "vulkaninstance.h"
#include "shadermodule.h"
#include "queuefamily.h"
#include "commandpoolmanager.h"
// Consider moving to cpp file too
#include "../stbi/stb_image.h"

#include "vertexobject.h"

namespace IceFairy {

	class VulkanModule : public Module {
	public:
		VulkanModule();
		virtual ~VulkanModule() { }

		std::string GetName(void) const;

		bool Initialise(void);
		void CleanUp(void);

		void StartMainLoop(void);

		// TODO: This might move elsewhere
		void AddVertexObject(const VertexObject& vertexObject);

		void SetWindowWidth(const int& windowWidth);
		void SetWindowHeight(const int& windowHeight);
		int GetWindowWidth(void) const;
		int GetWindowHeight(void) const;

		GLFWwindow* GetWindow(void);

		// TODO: Rethink how to do this - we don't want this public
		void SetIsFrameBufferResized(const bool& value);

	private:
		int windowWidth = -1;
		int windowHeight = -1;

		// TODO: config?
		const int MAX_FRAMES_IN_FLIGHT = 2;

		// Take note:
		// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap14.html#interfaces-resources-layout
		// Explanation bottom of this tutorial https://vulkan-tutorial.com/en/Uniform_buffers/Descriptor_pool_and_sets
		typedef struct {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		} UniformBufferObject;

		// TODO: smart pointer?
		GLFWwindow* window;
		std::shared_ptr<VulkanInstance> instance;

		// TODO: vk::UniqueSurfaceKHR
		VkSurfaceKHR surface;

		vk::PhysicalDevice physicalDevice;

		std::shared_ptr<VulkanDevice> device;

		vk::SwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		vk::Extent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;

		vk::RenderPass renderPass;
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline graphicsPipeline;

		std::vector<vk::Framebuffer> swapChainFramebuffers;

		std::shared_ptr<CommandPoolManager> commandPoolManager;
		// TODO: Handle this properly - See CommandPoolManager
		std::vector<vk::CommandBuffer> commandBuffers;

		std::vector<VertexObject> vertexObjects;

		std::vector<std::pair<vk::Buffer, vma::Allocation>> uniformBuffers;

		vk::DescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> descriptorSets;

		uint32_t mipLevels;
		vk::Image textureImage;
		vk::ImageView textureImageView;
		vk::Sampler textureSampler;
		vma::Allocation textureImageMemory;
		vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;
		vk::Image colorImage;
		vma::Allocation colorImageMemory;
		vk::ImageView colorImageView;

		void CheckPreconditions(void);

		// Todo: change this - change it to what??
		std::vector<vk::Semaphore> imageAvailableSemaphores;
		std::vector<vk::Semaphore> renderFinishedSemaphores;
		std::vector<vk::Fence> inFlightFences;
		size_t currentFrame = 0;

		bool isFrameBufferResized = false;

		// Depth buffering
		vk::Image depthImage;
		vma::Allocation depthImageMemory;
		vk::ImageView depthImageView;

		// Memory Allocator
		vma::Allocator allocator;

		// Base Initialisation
		void InitialiseWindow(void);
		void InitialiseVulkanInstance(void);
		void CreateSurface(void);
		void CreateSwapChain(void);

		void RunMainLoop(void);

		// Physical Device
		void PickPhysicalDevice(void);
		bool IsDeviceSuitable(vk::PhysicalDevice device);

		// Logical Device
		void CreateLogicalDevice(void);

		// Memory Allocator
		void CreateMemoryAllocator(void);

		// Swap chain
		void RecreateSwapChain(void);
		void CleanupSwapChain(void);

		// Image views
		void CreateImageViews(void);
		vk::ImageView CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);
		void CreateTextureImage(void);
		void CreateTextureImageView(void);
		void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vma::Allocation& imageMemory);
		void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels);
		void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
		void GenerateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		vk::SampleCountFlagBits GetMaxUsableSampleCount(void);
		void CreateColorResources(void);

		// Graphics pipeline
		void CreateGraphicsPipeline(void);

		// Render pass
		void CreateRenderPass(void);

		// Frame Buffers
		void CreateFrameBuffers(void);

		// Drawing
		void DrawFrame(void);
		void CreateTextureSampler(void);

		// Semaphores
		void CreateSyncObjects(void);

		// Buffers
		std::pair<vk::Buffer, vma::Allocation> CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
		void CopyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

		// Depth buffering
		void CreateDepthResources(void);
		vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
		vk::Format FindDepthFormat(void);
		bool HasStencilComponent(vk::Format format);

		// Vertex buffer
		void CreateVertexBuffer(VertexObject& vertexObject);
		void CreateIndexBuffer(VertexObject& vertexObject);
		uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void LoadModel(void);

		// Uniform buffer
		void CreateUniformBuffers(void);
		void UpdateUniformBuffer(uint32_t currentImage);

		bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);

		// Debugging
		vk::DebugUtilsMessengerEXT callback;

		void SetupDebugCallback(void);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			unsigned int logLevel;

			switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				logLevel = Logger::LEVEL_ERROR;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				logLevel = Logger::LEVEL_WARNING;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				logLevel = Logger::LEVEL_DEBUG;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			default:
				logLevel = Logger::LEVEL_TRACE;
				break;
			}

			Logger::PrintLn(logLevel, "Vulkan Validation Layer: '%s'", pCallbackData->pMessage);

			return VK_FALSE;
		}
	};

}
