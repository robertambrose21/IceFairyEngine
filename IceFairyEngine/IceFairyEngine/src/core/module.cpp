#include "module.h"

using namespace IceFairy;

Module::Module()
{ }

bool Module::Initialise(void) {
	for (auto& module : subModules) {
		if (!module.second->Initialise()) {
			Logger::PrintLn(Logger::LEVEL_ERROR, "\tSubModule [%s] FAILED.", module.first.c_str());
			return false;
		}
	}

	return true;
}

std::shared_ptr<Module> Module::AddSubModule(std::shared_ptr<Module> module) {
	try {
		subModules[module->GetName()] = module;
		return module;
	}
	catch (NoSuchModuleException& e) {
		Logger::PrintLn(Logger::LEVEL_ERROR, "\t[%s] module could not be loaded: %s", module->GetName().c_str(), e.what());
		return nullptr;
	}
}

std::string Module::GetName(void) const {
	return "No Module name defined - Implement \"GetName()\" in your module.";
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