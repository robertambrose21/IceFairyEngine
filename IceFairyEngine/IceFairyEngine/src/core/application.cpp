#include "application.h"

using namespace IceFairy;

Application::Application(int argc, char** argv)
    : argc(argc),
      argv(argv)
{ }

std::shared_ptr<Module> Application::LoadModule(std::string moduleName) {
    try {
        auto module = ModuleMapper::CreateModule(moduleName);
        module->Initialise();

        modules[moduleName] = module;

        Logger::Print(Logger::LEVEL_INFO, "[%s] module loaded.", moduleName.c_str());

        return module;
    }
    catch (NoSuchModuleException& e) {
        Logger::Print(Logger::LEVEL_ERROR, "[%s] module could not be loaded: %s", moduleName.c_str(), e.what());
        return nullptr;
    }
}

std::shared_ptr<Module> Application::GetModule(std::string moduleName) {
    if (IsModuleLoaded(moduleName)) {
        return modules[moduleName];
    }
    else {
        throw NoSuchModuleException();
    }
}

bool Application::IsModuleLoaded(std::string moduleName) {
    return modules.find(moduleName) != modules.end();
}

void Application::UnloadModule(std::string moduleName) {
    if (IsModuleLoaded(moduleName)) {
        modules.erase(moduleName);
        Logger::Print(Logger::LEVEL_WARNING, "[%s] module unloaded.", moduleName.c_str());
    }
}

int Application::GetArgc(void) const {
    return argc;
}

char** Application::GetArgv(void) const {
    return argv;
}