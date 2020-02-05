#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "vulkan/vulkanmodule.h"
#include "core/application.h"

#define ICE_FAIRY_TREAT_VERBOSE_AS_DEBUG true

class DemoApplication : IceFairy::Application {
public:
	DemoApplication(int argc, char** argv)
		: IceFairy::Application(argc, argv)
	{ }

	~DemoApplication() {
		module->CleanUp();
	}

	void Initialise(void) {
		module = AddModule<IceFairy::VulkanModule>("VulkanModule");

		Application::Initialise();
	}

	void StartMainLoop(void) {
		module->StartMainLoop();
	}

private:
	std::shared_ptr<IceFairy::VulkanModule> module;
};

int main(int argc, char** argv) {
 #ifdef NDEBUG
 	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_INFO);
 #else
 	IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_TRACE);
 #endif

	try {
		auto app = std::make_shared<DemoApplication>(argc, argv);

		app->Initialise();
		app->StartMainLoop();
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
		// IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_ERROR, e.what());
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
