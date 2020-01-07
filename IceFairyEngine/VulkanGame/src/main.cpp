//#ifdef _MSC_VER
//#define _CRTDBG_MAP_ALLOC  
//#include <crtdbg.h>
//#else
//#define _ASSERT(expr) ((void)0)
//#define _ASSERTE(expr) ((void)0)
//#endif

#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "vulkan/vulkanmodule.h"

#define ICE_FAIRY_TREAT_VERBOSE_AS_DEBUG true

int main(int argc, char** argv) {
	/*#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif*/

 //#ifdef NDEBUG
 //	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_INFO);
 //#else
 //	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_TRACE);
 //#endif

	/*auto app = std::make_shared<DemoApplication>(argc, argv);
	app->Initialise();
	app->Start();*/

	try {
		auto module = std::make_shared<IceFairy::VulkanModule>(IceFairy::VulkanModule("VulkanModule"));
		module->Initialise();
		module->StartMainLoop();
		module->CleanUp();
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
		// IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_ERROR, e.what());
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}



//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <stdexcept>
//#include <cstdlib>
//
//#include <vulkan/vulkan.hpp>
//
//#include "vulkan/vulkanmodule.h"
//
//const int WIDTH = 800;
//const int HEIGHT = 600;
//
//class HelloTriangleApplication {
//public:
//	void run() {
//		initWindow();
//		initVulkan();
//		mainLoop();
//		cleanup();
//	}
//
//private:
//	GLFWwindow* window;
//
//	void initWindow() {
//		glfwInit();
//
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
//	}
//
//	void initVulkan() {
//		createInstance();
//	}
//
//	void mainLoop() {
//		while (!glfwWindowShouldClose(window)) {
//			glfwPollEvents();
//		}
//	}
//
//	void cleanup() {
//		glfwDestroyWindow(window);
//
//		glfwTerminate();
//	}
//
//	void createInstance() {
//		vk::ApplicationInfo appInfo("Test", 1, "vulkan.hpp", VK_API_VERSION_1_1);
//
//		uint32_t glfwExtensionCount = 0;
//		const char** glfwExtensions;
//		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//		/*createInfo.enabledExtensionCount = glfwExtensionCount;
//		createInfo.ppEnabledExtensionNames = glfwExtensions;
//
//		createInfo.enabledLayerCount = 0;*/
//
//		vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(), &appInfo, 0, nullptr, glfwExtensionCount, glfwExtensions);
//		//vk::InstanceCreateInfo createInfo({}, &appInfo);
//
//		vk::UniqueInstance instance = vk::createInstanceUnique(createInfo);
//
//		/*if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create instance!");
//		}*/
//	}
//};
//
//int main() {
//	HelloTriangleApplication app;
//
//	try {
//		app.run();
//	}
//	catch (const std::exception& e) {
//		std::cerr << e.what() << std::endl;
//		return EXIT_FAILURE;
//	}
//
//	return EXIT_SUCCESS;
//}

