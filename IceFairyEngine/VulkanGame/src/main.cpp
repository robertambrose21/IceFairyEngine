#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)
#define _ASSERTE(expr) ((void)0)
#endif

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
	#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

 #ifdef NDEBUG
 	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_INFO);
 #else
 	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_TRACE);
 #endif

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

	std::cout << "Hey buddy" << std::endl;

	return EXIT_SUCCESS;
}
