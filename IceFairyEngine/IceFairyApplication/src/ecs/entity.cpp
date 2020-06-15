#include "entity.h"

IceFairy::Entity::Entity(int id) :
	id(id) {
}

std::shared_ptr<IceFairy::Component> IceFairy::Entity::GetComponent(const std::type_index& type) {
	if (components.find(type) == components.end()) {
		throw EntityException(id, "Couldn't find component type '" + COMPONENT_TYPE_NAMES[type] + "'");
	}

	return components[type];
}
