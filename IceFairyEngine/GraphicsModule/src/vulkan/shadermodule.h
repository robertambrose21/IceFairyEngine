#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>

#include "vulkanexception.h"
#include "core/utilities/logger.h"

namespace IceFairy {

	class ShaderModule {
	public:
		ShaderModule(VkDevice device);

		void LoadFromFile(const std::string& vertexShader, const std::string& fragmentShader);
		void CleanUp(void);
		VkPipelineShaderStageCreateInfo GetVertexShaderStageInfo(void) const;
		VkPipelineShaderStageCreateInfo GetFragmentShaderStageInfo(void) const;
		
	private:
		VkDevice device;
		VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
		bool isLoaded;

		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;

		static std::vector<char> ReadFile(const std::string& filename);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreatePipelineShaderStageCreateInfo(VkShaderModule vertexShaderModule, VkShaderModule fragmentShaderModule);
	};

};