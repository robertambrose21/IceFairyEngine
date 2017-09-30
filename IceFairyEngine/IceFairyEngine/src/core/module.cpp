#include "module.h"
#include "..\modules\modulemapper.h"

using namespace IceFairy;

Module::Module(std::string name)
    : name(name)
{ }

std::string Module::GetName(void) const {
    return name;
}

void Module::AddSubModule(std::string subModuleName) {
    auto module = CreateSubModule(subModuleName);
    module->Initialise();

    subModules[subModuleName] = module;

    Logger::Print(Logger::LEVEL_INFO, "[%s] submodule loaded for %s.", subModuleName.c_str(), GetName().c_str());
}

std::shared_ptr<Module> Module::CreateSubModule(std::string subModuleName) {
    return ModuleMapper::CreateModule(subModuleName);
}

std::string Module::ListSubModules(void) {
    if (subModules.empty()) {
        return "[]";
    }

    std::string list = "[";

    for (auto moduleItem : subModules) {
        auto module = moduleItem.second;
        list += module->GetName() + ", ";
    }

    return list.substr(0, list.length() - 2) + ']';
}