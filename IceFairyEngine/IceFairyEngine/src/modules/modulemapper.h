#ifndef __ice_fairy_module_mapper_h__
#define __ice_fairy_module_mapper_h__

#include "modulenames.h"
#include "../core/module.h"
#include "graphicsmodule.h"

namespace IceFairy {
    /*! \brief Functionality for mapping a \ref Module name to an object. */
    class ModuleMapper {
    public:
        /*! \brief Creates a module with a given name.
         *
         * \param moduleName The name of the module if it exists.
         * \returns The newly constructed module.
         * \throws NoSuchModuleException
         */
        static std::shared_ptr<Module> CreateModule(std::string moduleName);
    };
}

#endif /* __ice_fairy_module_mapper_h__ */