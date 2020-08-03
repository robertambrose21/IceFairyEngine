#pragma once

#include <set>

#include "vulkan/vulkan.hpp"
#include "memory-allocator/vk_mem_alloc.hpp"

#include "swapchainsupportdetails.h"
#include "validationlayers.h"
#include "queuefamily.h"
#include "vulkanexception.h"
#include "shadermodule.h"
#include "vertexobject.h"

namespace IceFairy {

	class VulkanDevice {
	public:
		VulkanDevice(
			const vk::PhysicalDevice& physicalDevice,
			const vk::SurfaceKHR& surface,
			QueueFamily::Indices indices
		);

		const vk::UniqueDevice& GetDevice(void);

		void CreateSwapChain(GLFWwindow* window);
		void RecreateSwapChain(GLFWwindow* window);
		void CleanupSwapChain(void);
		std::vector<vk::ImageView> CreateImageViews(void);

		vk::DescriptorPool CreateDescriptorPool(void);
		vk::DescriptorSetLayout CreateDescriptorSetLayout(void);

		// TODO: This is where we pass in uniform buffers, might need to extract this out/think about it some more
		std::vector<vk::DescriptorSet> CreateDescriptorSets(
			std::vector<std::pair<vk::Buffer, vma::Allocation>> uniformBuffers,
			vk::Sampler textureSampler,
			vk::ImageView textureImageView,
			vk::DeviceSize range
		);

		vk::ImageView CreateImageView(
			vk::Image image,
			vk::Format format,
			vk::ImageAspectFlags aspectFlags,
			uint32_t mipLevels
		);

		// TODO: Add more parameters we can pass in
		vk::Sampler CreateTextureSampler(const uint32_t& mipLevels);

		std::pair<vk::Image, vma::Allocation> CreateImage(
			uint32_t width,
			uint32_t height,
			uint32_t mipLevels,
			vk::SampleCountFlagBits numSamples,
			vk::Format format,
			vk::ImageTiling tiling,
			vk::ImageUsageFlags usage,
			vk::MemoryPropertyFlags properties,
			vma::Allocator allocator
		);

		std::pair< vk::PipelineLayout, vk::Pipeline> CreateGraphicsPipeline(
			vk::Extent2D swapChainExtent,
			vk::SampleCountFlagBits msaaSamples,
			vk::RenderPass renderPass
		);

		vk::RenderPass CreateRenderPass(
			vk::Format depthFormat,
			vk::Format swapChainImageFormat,
			vk::SampleCountFlagBits msaaSamples
		);

		std::vector<vk::Framebuffer> CreateFrameBuffers(
			vk::ImageView colorImageView,
			vk::ImageView depthImageView,
			vk::RenderPass renderPass
		);

		std::vector<vk::Fence> CreateSyncObjects(
			std::vector<vk::Semaphore>& imageAvailableSemaphores,
			std::vector<vk::Semaphore>& renderFinishedSemaphores,
			const int& maxFramesInFlight
		);

		void WaitIdle(void);
		void WaitIdleGraphicsQueue(void);

		vk::Extent2D GetSwapChainExtent(void) const;
		vk::Format GetSwapChainFormat(void) const;

		vk::Result AcquireNextSwapChainImage(
			const uint32_t& timeout,
			vk::Semaphore semaphore,
			vk::Fence fence,
			uint32_t* pImageIndex
		);

		vk::Result QueueImageForPresentation(
			std::vector<vk::Semaphore>& signalSemaphores,
			std::vector<uint32_t>& imageIndicies
		);

		void Submit(
			std::vector<vk::Semaphore>& waitSemaphores,
			std::vector<vk::PipelineStageFlags>& waitStages,
			std::vector<vk::Semaphore>& signalSemaphores,
			std::vector<vk::CommandBuffer>& commandBuffers,
			vk::Fence fence
		);

		void Submit(std::vector<vk::CommandBuffer>& commandBuffers);

		// TODO: Potentially remove later - only used for uniform buffers
		uint32_t GetNumSwapChainImages(void) const;

	private:
		vk::UniqueDevice CreateDevice(
			const vk::PhysicalDevice& physicalDevice,
			const vk::SurfaceKHR& surface,
			QueueFamily::Indices indices
		);

		const vk::UniqueDevice device;

		vk::PhysicalDevice physicalDevice;
		vk::SurfaceKHR surface;

		QueueFamily::Indices indices;

		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> descriptorSets;

		vk::SwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		vk::Extent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;
	};

}