#include "vulkaninstance.h"

IceFairy::VulkanInstance::VulkanInstance() :
	instance(CreateInstance()) {
}

const vk::UniqueInstance& IceFairy::VulkanInstance::GetInstance(void) {
	return instance;
}

vk::UniqueInstance IceFairy::VulkanInstance::CreateInstance(void) {
	if (enableValidationLayers && !CheckValidationLayerSupport()) {
		throw VulkanException("validation layers requested, but not available!");
	}

	try {
		uint32_t numLayers = 0;
		const char* const* layerNames = nullptr;
		auto extensions = GetRequiredExtensions();

		if (enableValidationLayers) {
			numLayers = static_cast<uint32_t>(validationLayers.size());
			layerNames = validationLayers.data();
		}

		vk::ApplicationInfo appInfo("Vulkan Demo", 0, "IceFairyEngine", 0, VK_API_VERSION_1_1);
		vk::InstanceCreateInfo createInfo({}, &appInfo, numLayers, layerNames, static_cast<uint32_t>(extensions.size()), extensions.data());

		return vk::createInstanceUnique(createInfo);;
	}
	catch (std::runtime_error& err) {
		throw VulkanException("failed to create instance: " + std::string(err.what()));
	}
}

std::vector<const char*> IceFairy::VulkanInstance::GetRequiredExtensions(void) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool IceFairy::VulkanInstance::CheckValidationLayerSupport(void) {
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
