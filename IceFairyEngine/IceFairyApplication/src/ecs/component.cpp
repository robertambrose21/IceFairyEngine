#include "component.h"

IceFairy::Component::Component(const char* name) :
	name(name)
{ }

const char* IceFairy::Component::GetName(void) const {
	return name;
}
