#pragma once

#include <map>

#include "component.h"

namespace IceFairy {

	class Entity {
	public:
		Entity();

		void AddComponent(const Component& component);
		Component& GetComponent(const char* name);

	private:
		std::map<const char*, Component> components;
		int id;
	};

}