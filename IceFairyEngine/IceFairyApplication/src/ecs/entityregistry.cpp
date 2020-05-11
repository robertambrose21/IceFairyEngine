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
	registeredModules[typeid(*module)] = module;
}

void IceFairy::EntityRegistry::Initialise(void) {
	if (IsModuleRegistered<VulkanModule>()) {
		auto module = GetRegisteredModule<VulkanModule>();

		Schedule(std::make_shared<VertexObjectCreationJob>(module));
	}
}

void IceFairy::EntityRegistry::StartEntityLoop(void) {
	for (auto& [id, entity] : entities) {
		// currently no-op
	}
}
