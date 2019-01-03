#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "core/module.h"

namespace IceFairy {
	class VulkanModule : public Module {
	public:
		VulkanModule(const std::string& name);
		virtual ~VulkanModule() { }

		bool Initialise(void);
		void StartMainLoop(void);

		void CleanUp(void);

	private:
		GLFWwindow* window;
		VkInstance instance;
		VkDebugUtilsMessengerEXT callback;

		void InitialiseWindow();
		void InitialiseVulkanInstance();

		void RunMainLoop(void);

		// Move to debugging file
		void setupDebugCallback();
		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}
	};
}