#include "swapchainsupportdetails.h"

IceFairy::SwapChainSupportDetails::SwapChainSupportDetails(const vk::SurfaceCapabilitiesKHR& capabilities,
	std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes) :
	capabilities(capabilities),
	formats(std::move(formats)),
	presentModes(std::move(presentModes)) {
}

vk::SurfaceFormatKHR IceFairy::SwapChainSupportDetails::ChooseSwapSurfaceFormat(void) {
	if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined) {
		return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}

	for (const auto& availableFormat : formats) {
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return availableFormat;
		}
	}

	return formats[0];
}

vk::PresentModeKHR IceFairy::SwapChainSupportDetails::ChooseSwapPresentMode(void) {
	vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

	for (const auto& availablePresentMode : presentModes) {
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		} else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

vk::Extent2D IceFairy::SwapChainSupportDetails::ChooseSwapExtent(GLFWwindow* window) {
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

vk::SurfaceCapabilitiesKHR IceFairy::SwapChainSupportDetails::GetCapabilities(void) const {
	return capabilities;
}

std::vector<vk::SurfaceFormatKHR> IceFairy::SwapChainSupportDetails::GetFormats(void) const {
	return formats;
}

std::vector<vk::PresentModeKHR> IceFairy::SwapChainSupportDetails::GetPresentModes(void) const {
	return presentModes;
}
