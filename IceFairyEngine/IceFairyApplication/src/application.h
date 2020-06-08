#pragma once

#include <unordered_map>

#include "core/module.h"
#include "core/utilities/logger.h"
#include "ecs/entityregistry.h"

namespace IceFairy {
	/*! \brief Abstract application class. Baseline for the main app entry point.
	 *
	 * Each application instantiation will need to overload the \ref Initialise class member to load
	 * any required modules.\n
	 * Additionally the destructor may be overloaded for any clean up.
	 * An example application is as follows:
	 * \code{.cpp}
	 * DemoApplication : public Application {
	 * public:
	 *     ~DemoApplication() { }
	 *
	 *     void Initialise(void) {
	 *         LoadModule(ICEFAIRY_GRAPHICS_MODULE);
	 *     }
	 * };
	 *
	 * int main() {
	 *     DemoApplication app;
	 *
	 *     app.Initialise();
	 *     app.Run();
	 *
	 *     return 0;
	 * }
	 * \endcode
	 */
	class Application {
	public:
		Application(int argc, char** argv);
		/*! \brief Destructor, overload for use of any application clean up. */
		virtual ~Application() { }

		/*! \brief Initialises the module and any sub-modules.
		*
		* Instantiated versions of this class member should be used for adding top level modules
		* and preparing any additional resources/configuration needed by the application.
		*/
		virtual void Initialise(void);
		/*! \brief Returns a module with a given name.
		 *
		 * \param moduleName The name of the module to return.
		 * \returns The module if it is loaded.
		 * \throws NoSuchModuleException.
		 */
		std::shared_ptr<Module> GetModule(std::string moduleName);

		/*! \brief Returns whether a module with a given name is loaded
		 *
		 * \returns True if the module is loaded.
		 */
		bool IsModuleLoaded(std::string moduleName);

		std::shared_ptr<EntityRegistry> GetEntityRegistry(void);

	protected:
		// TODO: Handle this better
		/*! \brief Adds a module into the application. */
		template<class T>
		std::shared_ptr<T> AddModule() {
			auto module = std::make_shared<T>();

			try {
				modules[module->GetName()] = module;
				entityRegistry->AddRegisteredModule(module);
				return module;
			}
			catch (NoSuchModuleException& e) {
				Logger::PrintLn(Logger::LEVEL_ERROR, "[%s] module could not be loaded: %s", module->GetName().c_str(), e.what());
				return nullptr;
			}
		}

		/*! \brief Removes a module with a given name from the application.
		 *
		 * \param moduleName The name of the module if it exists.
		 */
		void UnloadModule(std::string moduleName);

		int GetArgc(void) const;
		char** GetArgv(void) const;

	private:
		std::shared_ptr<EntityRegistry> entityRegistry;
		std::unordered_map<std::string, std::shared_ptr<Module>> modules;

		int argc;
		char** argv;
	};
}
