#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

#include "vulkanexception.h"
#include "queuefamily.h"
#include "vertexobject.h"
#include "vulkandevice.h"

namespace IceFairy {

	/*
	 * CommandPoolManager:
	 * - Apply all commands here for use in VulkanModule
	 * - Add helper vertex commands here
	 * - Pass this into VertexJobSystem
	 * - ApplicationContext might effect this
	*/
	class CommandPoolManager {
	public:
		CommandPoolManager(std::shared_ptr<VulkanDevice> deviceHandle, vk::PhysicalDevice physicalDeviceHandle, VkSurfaceKHR surfaceHandle);
		
		void CleanUp(void);

		// TODO: Reconsider passing through command buffer vector, maybe create a handle which contains a reference to the command buffer vector?
		// TODO: Lots of this stuff could be extracted from ApplicationContext?
		void CreateCommandBuffers(
			std::vector<vk::CommandBuffer>& commandBuffers,
			std::vector<VertexObject>& vertexObjects,
			std::vector<vk::Framebuffer>& swapChainFramebuffers,
			vk::RenderPass renderPass,
			vk::Extent2D swapChainExtent,
			vk::Pipeline graphicsPipeline,
			vk::PipelineLayout pipelineLayout,
			std::vector<vk::DescriptorSet>& descriptorSets);
		void FreeCommandBuffers(std::vector<vk::CommandBuffer>& commandBuffers);

		vk::CommandBuffer BeginSingleTimeCommands(void);
		void EndSingleTimeCommands(vk::CommandBuffer commandBuffer);

	private:
		void CreateCommandPool(void);

		std::shared_ptr<VulkanDevice> deviceHandle;
		vk::PhysicalDevice physicalDeviceHandle;
		VkSurfaceKHR surfaceHandle;

		vk::CommandPool commandPool;
	};

}
