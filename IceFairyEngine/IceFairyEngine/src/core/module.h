#ifndef __ice_fairy_module_h__
#define __ice_fairy_module_h__

#include <string>
#include <map>
#include <memory>
#include <exception>

#include "utilities\logger.h"
#include "utilities\icexception.h"

namespace IceFairy {
    /*! \brief Thrown when no module by a given name exists. */
    class NoSuchModuleException : public ICException {
    public:
        /*! \internal */
        NoSuchModuleException()
            : ICException("No module by that name exists.")
        { }
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
        /*! \brief STL map of Module pointers. */
        typedef std::map<std::string, std::shared_ptr<Module>> ModuleMap;

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
		virtual bool                    Initialise(void);
        /*! \brief Returns the name of this module.
         * 
         * \returns the name of this module.
         */
        virtual std::string             GetName(void) const;
        /*! \brief Lists the sub-modules of this module.
         *
         * \returns a string of all sub-modules used by this module.
         */
        std::string                     ListSubModules(void);

    protected:
        /*! \brief Adds a sub-module to this module.
         *
         * Adds a sub-module to this module - modules may have any layer of sub-modules.
         * \param module The sub-module to add.
         */
		std::shared_ptr<Module>			AddSubModule(std::shared_ptr<Module> module);

    private:
        ModuleMap   subModules;
    };
}

#endif /* __ice_fairy_module_h__ */