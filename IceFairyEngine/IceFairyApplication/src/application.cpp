#include "application.h"

using namespace IceFairy;

Application::Application(int argc, char** argv)
    : argc(argc),
      argv(argv)
{ }

void Application::Initialise() {
	const char* logo =
		"_\\ /_        ______   _                \n"
		" /_\\         |  ___| (_)               \n"
		" | | ___ ___ | |___ _ _ _ __ _   _      \n"
		" | |/ __/ _ \\  _/ _` | | '__| | | |    \n"
		" | | (_|  __/ || (_| | | |  | |_| |     \n"
		" |_|\\___\\___\\_| \\__,_|_|_|   \\__, |\n"
		"                              __/ |     \n"
		"                             |___/      \n\n"
		"___________________________________\n";

	Logger::Print(logo);
	Logger::PrintLn(Logger::LEVEL_INFO, "Loading Modules...");

	unsigned int numModulesLoaded = 0;
	for (auto& module : modules) {
		Logger::PrintLn(Logger::LEVEL_INFO, "[%s] Loading...", module.first.c_str());
		if (module.second->Initialise()) {
			Logger::PrintLn(Logger::LEVEL_INFO, "[%s] OK.", module.first.c_str());
			numModulesLoaded++;
		}
		else {
			Logger::PrintLn(Logger::LEVEL_ERROR, "[%s] FAILED.", module.first.c_str());
		}
	}
	Logger::PrintLn(Logger::LEVEL_INFO, "%d/%d modules loaded.", numModulesLoaded, modules.size());
	Logger::PrintLn(Logger::LEVEL_INFO, "Starting...");
}

std::shared_ptr<Module> Application::GetModule(std::string moduleName) {
    if (IsModuleLoaded(moduleName)) {
        return modules[moduleName];
    }

    throw NoSuchModuleException();
}

bool Application::IsModuleLoaded(std::string moduleName) {
    return modules.find(moduleName) != modules.end();
}

void Application::UnloadModule(std::string moduleName) {
    if (IsModuleLoaded(moduleName)) {
        modules.erase(moduleName);
        Logger::PrintLn(Logger::LEVEL_INFO, "[%s] module unloaded.", moduleName.c_str());
    }
}

int Application::GetArgc(void) const {
    return argc;
}

char** Application::GetArgv(void) const {
    return argv;
}