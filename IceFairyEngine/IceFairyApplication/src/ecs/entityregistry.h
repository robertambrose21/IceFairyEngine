#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "core/utilities/icexception.h"
#include "entity.h"
#include "core/module.h"
#include "vulkan/vulkanmodule.h"
#include "components/vertexobjectcomponent.h"

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

	private:
		std::unordered_map<std::string, std::shared_ptr<Module>> registeredModules;
		std::unordered_map<int, std::shared_ptr<Entity>> entities;
	};

}