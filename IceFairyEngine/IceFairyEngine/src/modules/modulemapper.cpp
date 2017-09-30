#include "modulemapper.h"

using namespace IceFairy;

std::shared_ptr<Module> ModuleMapper::CreateModule(std::string moduleName) {
    if (moduleName == ICEFAIRY_GRAPHICS_MODULE)
        return std::shared_ptr<Module>(new GraphicsModule());
    else
        throw NoSuchModuleException();
}