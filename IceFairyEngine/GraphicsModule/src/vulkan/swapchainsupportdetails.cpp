#include "swapchainsupportdetails.h"

IceFairy::SwapChainSupportDetails::SwapChainSupportDetails(const vk::SurfaceCapabilitiesKHR& capabilities,
	std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes) :
	capabilities(capabilities),
	formats(std::move(formats)),
	presentModes(std::move(presentModes))
{ }

vk::SurfaceCapabilitiesKHR IceFairy::SwapChainSupportDetails::GetCapabilities(void) const {
	return capabilities;
}

std::vector<vk::SurfaceFormatKHR> IceFairy::SwapChainSupportDetails::GetFormats(void) const {
	return formats;
}

std::vector<vk::PresentModeKHR> IceFairy::SwapChainSupportDetails::GetPresentModes(void) const {
	return presentModes;
}
