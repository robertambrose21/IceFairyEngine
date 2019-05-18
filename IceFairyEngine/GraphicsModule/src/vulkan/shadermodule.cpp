#include "shadermodule.h"

using namespace IceFairy;

ShaderModule::ShaderModule(VkDevice device)
	: device(device),
	  isLoaded(false)
{ }

void ShaderModule::LoadFromFile(const std::string& vertexShader, const std::string& fragmentShader) {
	Logger::PrintLn(Logger::LEVEL_TRACE, "Loading Vertex Shader: '%s'", vertexShader);
	auto vertexShaderCode = ReadFile(vertexShader);
	Logger::PrintLn(Logger::LEVEL_TRACE, "Loading Fragment Shader: '%s'", fragmentShader);
	auto fragmentShaderCode = ReadFile(fragmentShader);

	vertexShaderModule = CreateShaderModule(vertexShaderCode);
	fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

	CreatePipelineShaderStageCreateInfo(vertexShaderModule, fragmentShaderModule);

	/*vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	vkDestroyShaderModule(device, fragmentShaderModule, nullptr);*/

	isLoaded = true;
}

// TODO: Do we need this?
void ShaderModule::CleanUp(void) {
	vkDestroyShaderModule(device, vertexShaderModule, nullptr);
	vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
}

VkPipelineShaderStageCreateInfo ShaderModule::GetVertexShaderStageInfo(void) const {
	if (!isLoaded) {
		throw VulkanException("Cannot retrieve vertex shader stage info, shader has not been created yet");
	}

	return vertexShaderStageInfo;
}

VkPipelineShaderStageCreateInfo ShaderModule::GetFragmentShaderStageInfo(void) const {
	if (!isLoaded) {
		throw VulkanException("Cannot retrieve fragment shader stage info, shader has not been created yet");
	}

	return fragmentShaderStageInfo;
}

std::vector<char> ShaderModule::ReadFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw VulkanException("Failed to find shader with filename '"  + filename + "'");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

VkShaderModule ShaderModule::CreateShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

void ShaderModule::CreatePipelineShaderStageCreateInfo(VkShaderModule vertexShaderModule, VkShaderModule fragmentShaderModule) {
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module = vertexShaderModule;
	vertexShaderStageInfo.pName = "main";

	fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.module = fragmentShaderModule;
	fragmentShaderStageInfo.pName = "main";
}
