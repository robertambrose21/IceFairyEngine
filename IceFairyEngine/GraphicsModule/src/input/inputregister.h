#pragma once

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>

#include "core/utilities/logger.h"
#include "keylistener.h"
#include "mouselistener.h"

namespace IceFairy {
	class InputRegister {
	public:
		static void Initialise(GLFWwindow* window) {
			glfwSetKeyCallback(window, KeyListenerDispatch);
			glfwSetCursorPosCallback(window, MouseMovementListenerDispatch);
			glfwSetMouseButtonCallback(window, MouseButtonListenerDispatch);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		static void AddKeyListener(std::shared_ptr<KeyListener> keyListener) {
			GetInstance().AddKeyListenerInternal(keyListener);
		}

		static void AddMouseListener(std::shared_ptr<MouseListener> mouseListener) {
			GetInstance().AddMouseListenerInternal(mouseListener);
		}

		InputRegister(InputRegister const&) = delete;
		void operator=(InputRegister const&) = delete;

	private:
		InputRegister() { }

		static InputRegister& GetInstance() {
			static InputRegister instance;
			return instance;
		}

		static void KeyListenerDispatch(GLFWwindow *window, int key,
				int scancode, int action, int mods) {
			GetInstance().ListenToKeys(key, action, mods);
		}

		static void MouseMovementListenerDispatch(GLFWwindow* window,
				double xpos, double ypos) {
			GetInstance().ListenToMouseMovement(xpos, ypos);
		}

		static void MouseButtonListenerDispatch(GLFWwindow* window,
			int button, int action, int mods) {
			GetInstance().ListenToMouseButtons(button, action, mods);
		}

		void ListenToKeys(int key, int action, int mods);
		void AddKeyListenerInternal(std::shared_ptr<KeyListener> keyListener);

		void ListenToMouseMovement(double xpos, double ypos);
		void ListenToMouseButtons(int button, int action, int mods);
		void AddMouseListenerInternal(std::shared_ptr<MouseListener> mouseListener);

		GLFWwindow*									window;

		std::vector<std::shared_ptr<KeyListener>>	keyListeners;
		std::vector<std::shared_ptr<MouseListener>>	mouseListeners;
	};
}