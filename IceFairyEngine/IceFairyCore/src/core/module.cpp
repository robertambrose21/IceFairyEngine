#include "module.h"

using namespace IceFairy;

Module::Module() {
}

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
	throw ModuleNameUndefinedException();
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