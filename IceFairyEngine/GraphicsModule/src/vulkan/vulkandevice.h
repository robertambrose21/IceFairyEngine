#pragma once

#include <set>

#include "vulkan/vulkan.hpp"
#include "memory-allocator/vk_mem_alloc.hpp"

#include "swapchainsupportdetails.h"
#include "validationlayers.h"
#include "queuefamily.h"
#include "vulkanexception.h"

namespace IceFairy {

	class VulkanDevice {
	public:
		VulkanDevice(
			const vk::PhysicalDevice& physicalDevice,
			const vk::SurfaceKHR& surface,
			QueueFamily::Indices indices
		);

		const vk::UniqueDevice& GetDevice(void);
		vk::Queue GetGraphicsQueue(void);
		vk::Queue GetPresentQueue(void);

		SwapChainSupportDetails::Data CreateSwapChain(GLFWwindow* window);

		vk::DescriptorPool CreateDescriptorPool(const uint32_t& numSwapChainImages);
		vk::DescriptorSetLayout CreateDescriptorSetLayout(void);
		// TODO: This is where we pass in uniform buffers, might need to extract this out/think about it some more
		std::vector<vk::DescriptorSet> CreateDescriptorSets(
			const uint32_t& numSwapChainImages,
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

		void WaitIdle(void);

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
	};

}