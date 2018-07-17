#include "inputregister.h"

using namespace IceFairy;

void InputRegister::ListenToKeys(int key, int action, int mods) {
	for (auto keyListener : this->keyListeners) {
		switch (action) {
		case GLFW_PRESS:
			keyListener->OnKeyDown(key, mods);
			break;

		case GLFW_RELEASE:
			keyListener->OnKeyUp(key, mods);
			break;

		case GLFW_REPEAT:
			keyListener->OnKeyRepeat(key, mods);
			break;

		case GLFW_KEY_UNKNOWN:
		default:
			Logger::PrintLn(Logger::LEVEL_WARNING, 
					"Unknown key action '%s'. Key pressed: '%s' with modifiers '%s'",
					action, key, mods);
			break;
		}
	}
}

void InputRegister::ListenToMouseMovement(double xpos, double ypos) {
	for (auto mouseListener : this->mouseListeners) {
		mouseListener->OnMouseMovement(xpos, ypos);
	}
}

void InputRegister::ListenToMouseButtons(int button, int action, int mods) {
	for (auto mouseListener : this->mouseListeners) {
		switch (action) {
		case GLFW_PRESS:
			mouseListener->OnMouseButtonDown(button, mods);
			break;

		case GLFW_RELEASE:
			mouseListener->OnMouseButtonUp(button, mods);
			break;

		default:
			Logger::PrintLn(Logger::LEVEL_WARNING,
					"Unknown mouse button action '%s'. button pressed: '%s' with modifiers '%s'",
					action, button, mods);
			break;
		}
	}
}

void InputRegister::AddKeyListenerInternal(std::shared_ptr<KeyListener> keyListener) {
	this->keyListeners.push_back(keyListener);
}

void InputRegister::AddMouseListenerInternal(std::shared_ptr<MouseListener> mouseListener) {
	this->mouseListeners.push_back(mouseListener);
}