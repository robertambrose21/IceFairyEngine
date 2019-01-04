#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

#include "vulkanexception.h"
#include "core/utilities/logger.h"

namespace IceFairy {

#ifdef NDEBUG
	const bool ENABLE_VULKAN_VALIDATION_LAYERS = false;
#else
	const bool ENABLE_VULKAN_VALIDATION_LAYERS = true;
#endif

	const std::vector<const char*> VALIDATION_LAYERS = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
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

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

	void SetupDebugCallback(VkInstance instance, VkDebugUtilsMessengerEXT callback);

	bool CheckValidationLayerSupport();

}