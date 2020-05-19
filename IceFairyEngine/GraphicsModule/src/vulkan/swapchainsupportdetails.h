#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

namespace IceFairy {

	class SwapChainSupportDetails {
	public:
		SwapChainSupportDetails(const vk::SurfaceCapabilitiesKHR& capabilities,
			std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes);

		vk::SurfaceCapabilitiesKHR GetCapabilities(void) const;
		std::vector<vk::SurfaceFormatKHR> GetFormats(void) const;
		std::vector<vk::PresentModeKHR> GetPresentModes(void) const;

	private:
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

}