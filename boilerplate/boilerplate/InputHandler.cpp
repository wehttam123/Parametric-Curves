#define _USE_MATH_DEFINES

#include "InputHandler.h"
#include "Program.h"

RenderEngine* InputHandler::renderEngine;
int InputHandler::mouseOldX;
int InputHandler::mouseOldY;
bool InputHandler::mousePressed = false;

// Must be called before processing any GLFW events
void InputHandler::setUp(RenderEngine* renderEngine) {
	InputHandler::renderEngine = renderEngine;
}

// Callback for key presses
void InputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(0);
	}

	if (Program::rPressed) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			Program::r = Program::r + 0.01;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			Program::r = Program::r - 0.01;
		}
		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			Program::R = Program::R + 0.01;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			Program::R = Program::R - 0.01;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
			Program::r = Program::r + 0.01;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
			Program::r = Program::r - 0.01;
		}
		if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
			Program::R = Program::R + 0.01;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
			Program::R = Program::R - 0.01;
		}
	}
	else if (Program::nPressed) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			Program::n = Program::n + 1.0;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			Program::n = Program::n - 1.0;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
			Program::n = Program::n + 1.0;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
			Program::n = Program::n - 1.0;
		}
	}
	else {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			Program::speed = Program::speed * 1.1;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			Program::speed = Program::speed / 1.1;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
			Program::speed = Program::speed * 1.1;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
			Program::speed = Program::speed / 1.1;
		}
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		Program::rPressed = true;
	} 
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		Program::rPressed = false;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		Program::nPressed = true;
	}
	if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
		Program::nPressed = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		if (Program::hide) {
			Program::hide = false;
		} else {
			Program::hide = true;
		}
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if (Program::hide) {
			Program::hide = false;
		} else {
			Program::hide = true;
		}
	}
}

// Callback for mouse button presses
void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			mousePressed = true;
		else if (GLFW_RELEASE == action)
			mousePressed = false;
	}
}

// Callback for mouse motion
void InputHandler::motion(GLFWwindow* window, double x, double y) {
	if (mousePressed)
	{
		Program::angle = Program::angle + (((((x - mouseOldX)/50) + ((y - mouseOldY) / 50))/2) * (M_PI / 32));
	}
	mouseOldX = x;
	mouseOldY = y;


	//Program::angle = Program::angle + (y * (M_PI / 32));
}

// Callback for mouse scroll
void InputHandler::scroll(GLFWwindow* window, double x, double y) {
		Program::r = Program::r * (1 + (0.01 * y));
		Program::R = Program::R * (1 + (0.01 * y));
}

// Callback for window reshape/resize
void InputHandler::reshape(GLFWwindow* window, int width, int height) {
	renderEngine->setWindowSize(width, height);
	Program::width = width;
	Program::height = height;
}
