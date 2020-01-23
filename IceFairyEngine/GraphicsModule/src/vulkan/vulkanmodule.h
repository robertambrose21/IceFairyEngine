#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "vulkan/vulkan.hpp"

#include <string>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include <array>
#include <chrono>
#include <unordered_map>
// TODO: Offload to a resource class

#include "core/module.h"
#include "vulkanexception.h"
#include "shadermodule.h"
// Consider moving to cpp file too
#include "../stbi/stb_image.h"

namespace IceFairy {

	typedef struct _vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static vk::VertexInputBindingDescription getBindingDescription() {
			return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
		}

		static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

			attributeDescriptions[0] = vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos));
			attributeDescriptions[1] = vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color));
			attributeDescriptions[2] = vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord));

			return attributeDescriptions;
		}

		bool operator==(const _vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	} Vertex;

	class VulkanModule : public Module {
	public:
		VulkanModule(const std::string& name);
		virtual ~VulkanModule() { }

		bool Initialise(void);
		void CleanUp(void);

		void StartMainLoop(void);

		// TODO: Rethink how to do this - we don't want this public
		void SetIsFrameBufferResized(const bool& value);

	private:
		// TODO: Pass this in from somewhere - some config
		const int WIDTH = 800;
		const int HEIGHT = 600;

		// TODO: config?
		const int MAX_FRAMES_IN_FLIGHT = 2;

		typedef struct {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		} QueueFamilyIndices;

		typedef struct {
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR> presentModes;
		} SwapChainSupportDetails;

		// Take note:
		// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap14.html#interfaces-resources-layout
		// Explanation bottom of this tutorial https://vulkan-tutorial.com/en/Uniform_buffers/Descriptor_pool_and_sets
		typedef struct {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		} UniformBufferObject;

		GLFWwindow* window;
		vk::Instance instance;
		VkSurfaceKHR surface;

		vk::PhysicalDevice physicalDevice;
		vk::Device device;

		vk::Queue graphicsQueue;
		vk::Queue presentQueue;

		VkSwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;

		vk::RenderPass renderPass;
		vk::DescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		vk::CommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;
		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;

		std::vector<vk::Buffer> uniformBuffers;
		std::vector<vk::DeviceMemory> uniformBuffersMemory;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

		uint32_t mipLevels;
		vk::Image textureImage;
		vk::ImageView textureImageView;
		VkSampler textureSampler;
		vk::DeviceMemory textureImageMemory;
		vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;
		vk::Image colorImage;
		vk::DeviceMemory colorImageMemory;
		vk::ImageView colorImageView;

		// Todo: change this - change it to what??
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;

		bool isFrameBufferResized = false;

		// Depth buffering
		vk::Image depthImage;
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;

		// Base Initialisation
		void InitialiseWindow(void);
		void InitialiseVulkanInstance(void);
		void CreateSurface(void);
		void CreateSwapChain(void);

		void RunMainLoop(void);

		// Physical Device
		void PickPhysicalDevice(void);
		bool IsDeviceSuitable(vk::PhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device);

		// Logical Device
		void CreateLogicalDevice(void);

		// Swap chain
		SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device);
		vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes);
		vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
		void RecreateSwapChain(void);
		void CleanupSwapChain(void);

		// Image views
		void CreateImageViews(void);
		vk::ImageView CreateImageView(VkImage image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);
		void CreateTextureImage(void);
		void CreateTextureImageView(void);
		void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling,
			vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);
		vk::CommandBuffer BeginSingleTimeCommands(void);
		void EndSingleTimeCommands(vk::CommandBuffer commandBuffer);
		void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		vk::SampleCountFlagBits GetMaxUsableSampleCount(void);
		void CreateColorResources(void);

		// Graphics pipeline
		void CreateGraphicsPipeline(void);

		// Render pass
		void CreateRenderPass(void);

		// Frame Buffers
		void CreateFrameBuffers(void);

		// Commands
		void CreateCommandPool(void);
		void CreateCommandBuffers(void);

		// Drawing
		void DrawFrame(void);
		void CreateTextureSampler(void);

		// Semaphores
		void CreateSyncObjects(void);

		// Buffers
		void CreateBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
			vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		// Depth buffering
		void CreateDepthResources(void);
		vk::Format FindSupportedFormat(const std::vector<vk::Format>& candidates, VkImageTiling tiling, vk::FormatFeatureFlags features);
		vk::Format FindDepthFormat(void);
		bool HasStencilComponent(vk::Format format);

		// Vertex buffer
		void CreateVertexBuffer(void);
		void CreateIndexBuffer(void);
		uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void LoadModel(void);

		// Uniform buffer
		void CreateDescriptorSetLayout(void);
		void CreateUniformBuffers(void);
		void UpdateUniformBuffer(uint32_t currentImage);
		void CreateDescriptorPool(void);
		void CreateDescriptorSets(void);

		std::vector<const char*> GetRequiredExtensions(void);
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

namespace std {
	template<> struct hash<IceFairy::Vertex> {
		size_t operator()(IceFairy::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}