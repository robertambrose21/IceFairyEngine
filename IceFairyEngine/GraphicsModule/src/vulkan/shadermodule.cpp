#include "shadermodule.h"

using namespace IceFairy;

ShaderModule::ShaderModule(vk::Device device)
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

	isLoaded = true;
}

// TODO: Check if we can auto clean this up
void ShaderModule::CleanUp(void) {
	device.destroyShaderModule(vertexShaderModule, nullptr);
	device.destroyShaderModule(fragmentShaderModule, nullptr);
}

vk::PipelineShaderStageCreateInfo ShaderModule::GetVertexShaderStageInfo(void) const {
	if (!isLoaded) {
		throw VulkanException("Cannot retrieve vertex shader stage info, shader has not been created yet");
	}

	return vertexShaderStageInfo;
}

vk::PipelineShaderStageCreateInfo ShaderModule::GetFragmentShaderStageInfo(void) const {
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

vk::ShaderModule ShaderModule::CreateShaderModule(const std::vector<char>& code) {
	vk::ShaderModuleCreateInfo createInfo({}, code.size(), reinterpret_cast<const uint32_t*>(code.data()));

	try {
		return device.createShaderModule(createInfo);
	}
	catch (std::runtime_error err) {
		throw VulkanException("failed to create shader module!");
	}
}

void ShaderModule::CreatePipelineShaderStageCreateInfo(vk::ShaderModule vertexShaderModule, vk::ShaderModule fragmentShaderModule) {
	vertexShaderStageInfo = vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main");
	fragmentShaderStageInfo = vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main");
}
