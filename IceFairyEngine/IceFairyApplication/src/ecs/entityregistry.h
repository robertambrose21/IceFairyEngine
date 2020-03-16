#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "core/utilities/icexception.h"
#include "constants/modulenames.h"
#include "entity.h"
#include "core/module.h"
#include "vulkan/vulkanmodule.h"
#include "components/vertexobjectcomponent.h"
#include "jobsystem.h"
#include "systems/vertexobjectsystem.h"

namespace IceFairy {

	class EntityRegistryException : public ICException {
	public:
		EntityRegistryException(const std::string& message)
			: ICException("EntityRegistry encountered an error: " + message)
		{ }
	};

	class EntityRegistry {
	public:
		EntityRegistry();
		~EntityRegistry();

		std::shared_ptr<Entity> AddEntity(void);
		std::shared_ptr<Entity> GetEntity(int id);

		void AddRegisteredModule(std::shared_ptr<Module> module);

		void Initialise(void);
		void StartEntityLoop(void);

		// TODO: Multithreading and consider moving to a special JobSystem class
		template<typename... Ts>
		void Schedule(std::shared_ptr<JobSystem<Ts...>> system) {
			for (auto[id, entity] : entities) {
				if ((entity->HasComponent<Ts>() && ...)) {
					system->Execute(entity->GetComponent<Ts>()...);
				}
			}
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Module>> registeredModules;
		std::unordered_map<int, std::shared_ptr<Entity>> entities;
	};

}