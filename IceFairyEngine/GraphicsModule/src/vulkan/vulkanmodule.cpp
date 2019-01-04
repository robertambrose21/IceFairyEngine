#include "vulkanmodule.h"

using namespace IceFairy;

VulkanModule::VulkanModule(const std::string & name)
	: Module(name)
{ }

bool VulkanModule::Initialise(void) {
	InitialiseWindow();

	InitialiseVulkanInstance();
	SetupDebugCallback(instance, callback);

	return true;
}

void VulkanModule::StartMainLoop(void) {
	RunMainLoop();
}

void VulkanModule::InitialiseWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

void VulkanModule::InitialiseVulkanInstance() {
	if (ENABLE_VULKAN_VALIDATION_LAYERS && !CheckValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Demo"; // TODO: Sensible name 
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "IceFairyEngine"; // TODO: Extract from somewhere sensible
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (ENABLE_VULKAN_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw VulkanException("failed to create instance!");
	}
}

std::vector<const char*> VulkanModule::GetRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (ENABLE_VULKAN_VALIDATION_LAYERS) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanModule::RunMainLoop(void) {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void VulkanModule::CleanUp(void) {
	if (ENABLE_VULKAN_VALIDATION_LAYERS) {
		DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
	}

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}
