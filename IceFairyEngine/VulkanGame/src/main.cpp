#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <memory>

#include "vulkan/vulkanmodule.h"
#include "vulkan/vertexobject.h"
#include "application.h"

#include "input/inputregister.h"
#include "input/keylistener.h"

#include "ecs/components/vertexobjectcomponent.h"
#include "ecs/entityregistry.h"

#define ICE_FAIRY_TREAT_VERBOSE_AS_DEBUG true

/*
 * TODOs:
 * - Cleanup all TODOs
 * - Module names in a central place
 * - Don't use a staging buffer for vertices which change value but have a static count
 * - Create new buffers if the number of vertices change
 * - Break out material computation (textures)
 * - Use vulkan tools shaders to automatically compile shaders
 * - Store index/vertex buffers in a single VkBuffer
 * - Store submit/copy functions in a single command buffer and execute asynchronously
 * - Remove all instances of "using namespace IceFairy"
 * - Tests
 * - Device unique instance if possible
 */

class DemoApplication :
	public IceFairy::Application,
	public IceFairy::KeyListener,
	public std::enable_shared_from_this<DemoApplication> {
public:
	DemoApplication(int argc, char** argv)
		: IceFairy::Application(argc, argv)
	{ }

	~DemoApplication() {
		module->CleanUp();
	}

	void Initialise(void) {
		module = AddModule<IceFairy::VulkanModule>();

		module->SetWindowWidth(800);
		module->SetWindowHeight(600);

		auto entity1 = this->GetEntityRegistry().AddEntity();
		auto entity2 = this->GetEntityRegistry().AddEntity();

		entity1->AddComponent<IceFairy::VertexObjectComponent>(
			std::vector<unsigned int> {
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4
			},
			std::vector<IceFairy::Vertex> {
				{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
				{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
				{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
				{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

				{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
				{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
				{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
				{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
			}
		);

		entity2->AddComponent<IceFairy::VertexObjectComponent>(
			std::vector<unsigned int> {
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4
			},
			std::vector<IceFairy::Vertex> {
				{ {-0.5f, -0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
				{ {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
				{ {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
				{ {-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },

				{ {-0.5f, -0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
				{ {0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
				{ {0.5f, 0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
				{ {-0.5f, 0.5f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }
			}
		);

		Application::Initialise();

		IceFairy::InputRegister::Initialise(module->GetWindow());
		IceFairy::InputRegister::AddKeyListener(shared_from_this());
	}

	void StartMainLoop(void) {
		module->StartMainLoop();
	}

	void OnKeyDown(int key, int mods) {
		IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_INFO, "Key down '%c'", key);
	}

	void OnKeyUp(int key, int mods) {
		IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_INFO, "Key up '%c'", key);
	}

	void OnKeyRepeat(int key, int mods) {

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
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "m8 you left both logging on" << std::endl;
		//IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_ERROR, e.what());
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
