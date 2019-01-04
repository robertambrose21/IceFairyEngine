#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "core/module.h"
#include "validationlayers.h"
#include "vulkanexception.h"

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

		std::vector<const char*> GetRequiredExtensions();

		void RunMainLoop(void);
	};
}