#include "entityregistry.h"

IceFairy::EntityRegistry::EntityRegistry() {
}

IceFairy::EntityRegistry::~EntityRegistry() {
}

std::shared_ptr<IceFairy::Entity> IceFairy::EntityRegistry::AddEntity(void) {
	static int id = 0;
	entities[id] = std::make_shared<Entity>(id++);
	return entities[id];
}

std::shared_ptr<IceFairy::Entity> IceFairy::EntityRegistry::GetEntity(int id) {
	if (entities.find(id) == entities.end()) {
		throw EntityRegistryException("Cannot find entity with id '" + std::to_string(id) + "'");
	}

	return entities[id];
}

void IceFairy::EntityRegistry::AddRegisteredModule(std::shared_ptr<Module> module) {
	registeredModules[module->GetName()] = module;
}

void IceFairy::EntityRegistry::Initialise(void) {
	if (registeredModules.find("VulkanModule") != registeredModules.end()) {
		auto module = std::dynamic_pointer_cast<VulkanModule>(registeredModules["VulkanModule"]);

		Schedule(std::make_shared<VertexObjectSystem>(module));
	}
}

void IceFairy::EntityRegistry::StartEntityLoop(void) {
	for (auto& [id, entity] : entities) {
		// TODO: Common place for module names for easier lookup
		if (registeredModules.find("VulkanModule") != registeredModules.end()) {

		}
	}
}
