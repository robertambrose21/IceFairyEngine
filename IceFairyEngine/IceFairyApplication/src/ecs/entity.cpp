#include "entity.h"

IceFairy::Entity::Entity() {
}

void IceFairy::Entity::AddComponent(const Component& component) {
	components[component.GetName()] = component;
}

IceFairy::Component& IceFairy::Entity::GetComponent(const char* name) {
	return components[name];
}
