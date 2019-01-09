#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>

#include "core/module.h"
#include "vulkanexception.h"

namespace IceFairy {
	class VulkanModule : public Module {
	public:
		VulkanModule(const std::string& name);
		virtual ~VulkanModule() { }

		bool Initialise(void);
		void CleanUp(void);

		void StartMainLoop(void);

	private:
		// TODO: change this
		const int WIDTH = 800;
		const int HEIGHT = 600;

		typedef struct {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		} QueueFamilyIndices;

		typedef struct {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		} SwapChainSupportDetails;

		GLFWwindow* window;
		VkInstance instance;
		VkSurfaceKHR surface;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;

		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		// Base Initialisation
		void InitialiseWindow();
		void InitialiseVulkanInstance();
		void CreateSurface();
		void CreateSwapChain();

		void RunMainLoop(void);

		// Physical Device
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		// Logical Device
		void CreateLogicalDevice();

		// Swap chain
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		std::vector<const char*> GetRequiredExtensions();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		// Debugging
		VkDebugUtilsMessengerEXT callback;

		void SetupDebugCallback();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			unsigned int logLevel;

			switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				logLevel = Logger::LEVEL_ERROR;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				logLevel = Logger::LEVEL_WARNING;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				logLevel = Logger::LEVEL_DEBUG;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			default:
				logLevel = Logger::LEVEL_TRACE;
				break;
			}

			Logger::PrintLn(logLevel, "Vulkan Validation Layer: '%s'", pCallbackData->pMessage);

			return VK_FALSE;

		}
	};
}