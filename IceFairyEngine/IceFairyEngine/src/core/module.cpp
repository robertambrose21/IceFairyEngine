#include "module.h"

using namespace IceFairy;

const std::string DEFAULT_MODULE_WARNING_NAME = "No Module name defined - Implement \"GetName()\" in your module.";

Module::Module()
	: name(DEFAULT_MODULE_WARNING_NAME)
{ }

Module::Module(const std::string& name)
	: name(name)
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

std::string Module::GetName(void) const {
	return name;
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