#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>

#include "component.h"
#include "componenttypes.h"
#include "components/vertexobjectcomponent.h"
#include "core/utilities/icexception.h"

// TODO: Sub-entities
// TODO: Components should have no logic, only data. Construct systems which can take multiple components and filter out entities based off of them
namespace IceFairy {

	class EntityException : public ICException {
	public:
		EntityException(const int& entityId, const std::string& message)
			: ICException("Entity[" + std::to_string(entityId) + "] encountered an error: " + message)
		{ }
	};


	class Entity {
	public:
		Entity(int id);

		template<typename T, typename... Args, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		std::shared_ptr<T> AddComponent(Args&&... args) {
			auto component = std::make_shared<T>(std::forward<Args>(args)...);
			components[typeid(T)] = component;
			return component;
		}

		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		inline std::shared_ptr<T> GetComponent(void) {
			return std::static_pointer_cast<T>(GetComponent(typeid(T)));
		}

		template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		inline bool HasComponent(void) const {
			return components.find(typeid(T)) != components.end();
		}

		std::shared_ptr<Component> GetComponent(const std::type_index& type);

	private:
		std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
		int id;
	};

}