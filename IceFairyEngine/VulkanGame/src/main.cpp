#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "vulkan/vulkanmodule.h"
#include "vulkan/vertexobject.h"
#include "application.h"

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

		module->SetWindowWidth(800);
		module->SetWindowHeight(600);

		module->AddVertexObject(
			IceFairy::VertexObject(
				{
					0, 1, 2, 2, 3, 0,
					4, 5, 6, 6, 7, 4
				},
				{
					{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
					{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
					{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
					{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

					{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
					{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
					{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
					{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
				}
			)
		);
		module->AddVertexObject(
			IceFairy::VertexObject(
				{
					0, 1, 2, 2, 3, 0,
					4, 5, 6, 6, 7, 4
				},
				{
					{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
					{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
					{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
					{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

					{{-0.5f, -0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
					{{0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
					{{0.5f, 0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
					{{-0.5f, 0.5f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
				}
			)
		);

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
		std::cout << "m8 you left both logging on" << std::endl;
		//IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_ERROR, e.what());
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
