#ifndef __ice_fairy_module_h__
#define __ice_fairy_module_h__

#include <string>
#include <unordered_map>
#include <memory>
#include <exception>

#include "utilities\logger.h"
#include "utilities\icexception.h"

namespace IceFairy {
	/*! \brief Thrown when no module by a given name exists. */
	class NoSuchModuleException : public ICException {
	public:
		/*! \internal */
		NoSuchModuleException() :
			ICException("No module by that name exists.") {
		}
	};

	class ModuleNameUndefinedException : public ICException {
	public:
		/*! \internal */
		ModuleNameUndefinedException() :
			ICException("No Module name defined - Implement \"GetName()\" in your module.") {
		}
	};

	/*! \brief Abstract module class. Baseline for loading specific resources into an \ref Application
	 *
	 * Each module must be supplied with a name, preferably from \c modulenames.h and loaded with the
	 * module mapper (\ref ModuleMapper).\n
	 * There are two virtual class members (\ref Initialise and \ref Run) which \b must be instantiated.
	 * Additionally, the destructor may be is virtual and may be overloaded for cleaning up any
	 * resources used by the module.\n
	 * Modules may have a layer of submodules attached (loaded via \ref AddSubModule).\n
	 * An example of a module class is as follows:
	 * \code{.cpp}
	 * class MyModule : public IceFairy::Module {
	 * public:
	 *     MyModule()
	 *         : IceFairyModule(MY_MODULE_NAME)
	 *     { }
	 *
	 *     void Initialise(void) {
	 *         // Add any sub-modules needed by my module.
	 *         AddSubModule(MY_SUB_MODULE_NAME);
	 *     }
	 *
	 *     void Run(void) {
	 *         // ... Do stuff with my module here ...
	 *     }
	 * }
	 * \endcode
	 */
	class Module {
	public:
		/*! \brief Constructor, create the module */
		Module();
		/*! \brief Destructor, overload for use of any module clean up. */
		virtual ~Module() { };

		/*! \brief Initialises the module and any sub-modules.
		 *
		 * Instantiated versions of this class member should be used for adding sub-modules
		 * and preparing any resources needed for this moudle.
		 *
		 * \returns true if initialisation was successful
		 */
		virtual bool            Initialise(void);
		/*! \brief Returns the name of this module.
		 *
		 * \returns the name of this module.
		 */
		virtual std::string GetName(void) const;
		/*! \brief Lists the sub-modules of this module.
		 *
		 * \returns a string of all sub-modules used by this module.
		 */
		std::string             ListSubModules(void);

	protected:
		/*! \brief Adds a sub-module to this module.
		 *
		 * Adds a sub-module to this module - modules may have any layer of sub-modules.
		 * \param module The sub-module to add.
		 */
		template<class T>
		std::shared_ptr<T> AddSubModule(const std::string& name) {
			auto module = std::shared_ptr<T>(new T(name));

			try {
				subModules[module->GetName()] = module;
				return module;
			}
			catch (NoSuchModuleException& e) {
				Logger::PrintLn(Logger::LEVEL_ERROR, "[%s] sub-module could not be loaded: %s", module->GetName().c_str(), e.what());
				return nullptr;
			}
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Module>> subModules;
		std::string	name;
	};
}

#endif /* __ice_fairy_module_h__ */