#pragma once

#include "glfw3_include.h"

#include <vector>

#include "vulkan/vulkan.hpp"

namespace IceFairy {

	class SwapChainSupportDetails {
	public:
		typedef std::tuple<vk::SwapchainKHR, std::vector<vk::Image>, vk::Format, vk::Extent2D> Data;

		SwapChainSupportDetails(const vk::SurfaceCapabilitiesKHR& capabilities,
			std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes);

		static SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR surface) {
			return SwapChainSupportDetails(
				physicalDevice.getSurfaceCapabilitiesKHR(surface),
				physicalDevice.getSurfaceFormatsKHR(surface),
				physicalDevice.getSurfacePresentModesKHR(surface));
		}

		vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(void);
		vk::PresentModeKHR ChooseSwapPresentMode(void);
		vk::Extent2D ChooseSwapExtent(GLFWwindow* window);

		vk::SurfaceCapabilitiesKHR GetCapabilities(void) const;
		std::vector<vk::SurfaceFormatKHR> GetFormats(void) const;
		std::vector<vk::PresentModeKHR> GetPresentModes(void) const;

	private:
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

}