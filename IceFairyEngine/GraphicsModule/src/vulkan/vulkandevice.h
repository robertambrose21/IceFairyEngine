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
		VulkanDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
			QueueFamily::Indices indices);

		const vk::UniqueDevice& GetDevice(void);
		vk::Queue GetGraphicsQueue(void);
		vk::Queue GetPresentQueue(void);

		SwapChainSupportDetails::Data CreateSwapChain(GLFWwindow* window);

		vk::DescriptorPool CreateDescriptorPool(const uint32_t& numSwapChainImages);
		vk::DescriptorSetLayout CreateDescriptorSetLayout(void);
		std::vector<vk::DescriptorSet> CreateDescriptorSets(const uint32_t& numSwapChainImages,
			std::vector<std::pair<vk::Buffer, vma::Allocation>> uniformBuffers, vk::Sampler textureSampler,
			vk::ImageView textureImageView, vk::DeviceSize range);

	private:

		vk::UniqueDevice CreateDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
			QueueFamily::Indices indices);

		const vk::UniqueDevice device;

		vk::PhysicalDevice physicalDevice;
		vk::SurfaceKHR surface;

		QueueFamily::Indices indices;

		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorPool descriptorPool;
		std::vector<vk::DescriptorSet> descriptorSets;
	};

}