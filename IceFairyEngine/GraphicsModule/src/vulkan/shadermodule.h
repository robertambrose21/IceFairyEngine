#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/vulkan.hpp"

#include <string>
#include <vector>
#include <fstream>

#include "vulkanexception.h"
#include "core/utilities/logger.h"

namespace IceFairy {

	class ShaderModule {
	public:
		ShaderModule(vk::Device device);

		void LoadFromFile(const std::string& vertexShader, const std::string& fragmentShader);
		void CleanUp(void);
		vk::PipelineShaderStageCreateInfo GetVertexShaderStageInfo(void) const;
		vk::PipelineShaderStageCreateInfo GetFragmentShaderStageInfo(void) const;
		
	private:
		vk::Device device;
		vk::PipelineShaderStageCreateInfo vertexShaderStageInfo;
		vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo;
		bool isLoaded;

		vk::ShaderModule vertexShaderModule;
		vk::ShaderModule fragmentShaderModule;

		static std::vector<char> ReadFile(const std::string& filename);
		vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreatePipelineShaderStageCreateInfo(vk::ShaderModule vertexShaderModule, vk::ShaderModule fragmentShaderModule);
	};

};