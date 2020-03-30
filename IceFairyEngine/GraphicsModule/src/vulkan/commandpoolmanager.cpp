#include "commandpoolmanager.h"

IceFairy::CommandPoolManager::CommandPoolManager(vk::Device deviceHandle, vk::PhysicalDevice physicalDeviceHandle, VkSurfaceKHR surfaceHandle) :
	deviceHandle(deviceHandle),
	physicalDeviceHandle(physicalDeviceHandle),
	surfaceHandle(surfaceHandle)
{
	CreateCommandPool();
}

void IceFairy::CommandPoolManager::CleanUp(void) {
	if (commandPool) {
		vkDestroyCommandPool(deviceHandle, commandPool, nullptr);
	}
}

void IceFairy::CommandPoolManager::CreateCommandBuffers(
		std::vector<vk::CommandBuffer>& commandBuffers,
		std::vector<VertexObject>& vertexObjects,
		std::vector<vk::Framebuffer>& swapChainFramebuffers,
		vk::RenderPass renderPass,
		vk::Extent2D swapChainExtent,
		vk::Pipeline graphicsPipeline,
		vk::PipelineLayout pipelineLayout,
		std::vector<vk::DescriptorSet>& descriptorSets) {
	commandBuffers.resize(swapChainFramebuffers.size());

	vk::CommandBufferAllocateInfo allocInfo(commandPool, vk::CommandBufferLevel::ePrimary, (uint32_t)commandBuffers.size());

	commandBuffers = deviceHandle.allocateCommandBuffers(allocInfo);

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		try {
			commandBuffers[i].begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to begin recording command buffer!");
		}

		std::array<vk::ClearValue, 2> clearValues = {
			vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f})),
			vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))
		};

		vk::RenderPassBeginInfo renderPassInfo(renderPass, swapChainFramebuffers[i], vk::Rect2D({ 0, 0 }, swapChainExtent),
			static_cast<uint32_t>(clearValues.size()), clearValues.data());

		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

		for (auto& vertexObject : vertexObjects) {
			// Bind vertices and indices - https://www.reddit.com/r/vulkan/comments/69qqe0/variable_number_of_vertex_buffers/
			commandBuffers[i].bindVertexBuffers(0, { vertexObject.GetVertexBuffer() }, { 0 });
			commandBuffers[i].bindIndexBuffer(vertexObject.GetIndexBuffer(), 0, vk::IndexType::eUint32);
			// Bind 'descriptor sets' textures etc?
			commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { descriptorSets[i] }, nullptr);

			commandBuffers[i].drawIndexed(static_cast<uint32_t>(vertexObject.GetIndices().size()), 1, 0, 0, 0);
		}

		commandBuffers[i].endRenderPass();

		try {
			commandBuffers[i].end();
		}
		catch (std::runtime_error err) {
			throw VulkanException("failed to record command buffer!");
		}
	}
}

void IceFairy::CommandPoolManager::FreeCommandBuffers(std::vector<vk::CommandBuffer>& commandBuffers) {
	deviceHandle.freeCommandBuffers(commandPool, commandBuffers);
}

vk::CommandBuffer IceFairy::CommandPoolManager::BeginSingleTimeCommands(void) {
	vk::CommandBufferAllocateInfo allocInfo(commandPool, vk::CommandBufferLevel::ePrimary, 1);

	// TODO: Supposedly you allocate all buffers at the start? - This is possibly fine due to being a "single time command"
	vk::CommandBuffer commandBuffer = deviceHandle.allocateCommandBuffers(allocInfo).at(0);
	commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

	return commandBuffer;
}

void IceFairy::CommandPoolManager::EndSingleTimeCommands(vk::CommandBuffer commandBuffer, vk::Queue graphicsQueue) {
	commandBuffer.end();

	vk::SubmitInfo submitInfo = {};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	graphicsQueue.submit(1, &submitInfo, nullptr);
	graphicsQueue.waitIdle();

	deviceHandle.freeCommandBuffers(commandPool, 1, &commandBuffer);
}

void IceFairy::CommandPoolManager::CreateCommandPool(void) {
	QueueFamily::Indices queueFamilyIndices = QueueFamily(physicalDeviceHandle, surfaceHandle).FindQueueFamilies();
	
	try {
		commandPool = deviceHandle.createCommandPool(vk::CommandPoolCreateInfo({}, queueFamilyIndices.graphicsFamily.value()));
	}
	catch (std::runtime_error err) {
		// TODO: custom exception
		throw std::runtime_error("failed to create command pool!");
	}
}
