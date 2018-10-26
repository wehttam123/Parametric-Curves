#define _USE_MATH_DEFINES

#include "Program.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int Program::width;
int Program::height;

bool Program::hide;
double Program::speed;
float Program::r;
float Program::R;
float Program::n;
float Program::angle;
bool Program::rPressed;
bool Program::nPressed;

Program::Program() {
	window = nullptr;
	renderEngine = nullptr;
}

// Error callback for glfw errors
void Program::error(int error, const char* description) {
	std::cerr << description << std::endl;
}

// Called to start the program
void Program::start() {
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << std::endl;
	}

	Program::rPressed = false;
	Program::nPressed = false;

	cout << "--Hypocycloid Simulator--\n\n";

	cout << "To increase/decrease the radius of the small circle,\n";
	cout << "Hold down (r) and use the left and right arrow keys.\n\n";

	cout << "To increase/decrease the radius of the large circle,\n";
	cout << "Hold down (r) and use the up and down arrow keys.\n\n";

	cout << "To increase/decrease the number of cycles,\n";
	cout << "Hold down (n) and use the left and right arrow keys.\n\n";

	cout << "To rotate the hypocycloid, click and drag with the mouse.\n\n";

	cout << "To scale the hypocycloid, scroll up and down.\n\n";

	cout << "To increase/decrease the speed of the animation use the left and right arrow keys.\n\n";

	cout << "To hide/display the animation press (d) or (h).\n\n";

	renderEngine = new RenderEngine(window);
	InputHandler::setUp(renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow() {
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0) {
		exit(EXIT_FAILURE);
	}

	Program::width = 1280;
	Program::height = 720;

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(Program::width, Program::height, "CPSC 589 Assignment #1 Hypocycloid", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // V-sync on

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);
}

// Main loop
void Program::mainLoop() {

	Program::r = 4.0;	// Radius of small circle
	Program::R = 6.0;	// Radius of larger circle
	Program::n = 2.0;	// Number of cycles

	Program::angle = 0.0;		// Rotation Angle
	float scale = 1.0;		// Scale of Hypocycloid
	double i = 0.0;			// Position along cycle
	double u = 0.0;			// Position along Hypocycloid
	Program::speed = 0.01;	// Animation speed

	float x;	// X position
	float y;	// Y position
	float xr;	// X position rotated
	float yr;	// Y position rotated
	float xrm;	// X position rotated at middle of small circle
	float yrm;	// Y position rotated at middle of small circle

	Program::hide = false;

	// Adjust scale
	Program::r = Program::r * scale;
	Program::R = Program::R * scale;

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glLineWidth(3);

		// Hide
		HideCircle = Geometry::makeCircle(100.0, 0.0, 0.0, 1.0, 1.0, 1.0, Program::hide);

		// Animation Position
		if (i < Program::n) { 
			i += Program::speed;
		} else { 
			i = 0.0; 
		}

		// Build Hypocycloid
		Hypocycloid = Geometry::makeHypocycloid(Program::r, Program::R, i, Program::angle);


		/*** Calculations for Additional objects in scene ***/

		// Calculate Position
		u = 2.0 * M_PI * i;
		// Parametric Equations
		x = ((Program::R - Program::r) * cos(u)) + (Program::r * cos(((Program::R - Program::r) / Program::r) * u));
		y = ((Program::R - Program::r) * sin(u)) - (Program::r * sin(((Program::R - Program::r) / Program::r) * u));
		// Adjust rotation
		xr = (x * cos(Program::angle)) - (y * sin(Program::angle));
		yr = (y * cos(Program::angle)) + (x * sin(Program::angle));
		xrm = ((Program::R - Program::r)*cos(u) * cos(Program::angle)) - ((Program::R - Program::r)*sin(u) * sin(Program::angle));
		yrm = ((Program::R - Program::r)*sin(u) * cos(Program::angle)) + ((Program::R - Program::r)*cos(u) * sin(Program::angle));

		// Build Objects
		OuterCircle = Geometry::makeCircle(Program::R, 0.0, 0.0, 1.0, 0.5, 0.0, false);
		InnerCircle = Geometry::makeCircle(Program::r, xrm, yrm, 0.0, 0.0, 0.0, false);
		MiddleDot = Geometry::makeCircle(0.2, xrm, yrm, 0.0, 0.0, 0.0, true);
		Radius = Geometry::makeLine(xr, yr, xrm, yrm);
		EdgeDot = Geometry::makeCircle(0.2, xr, yr, 1.0, 0.0, 0.0, true);

		// Render Objects
		renderEngine->assignBuffers(HideCircle);
		renderEngine->updateBuffers(HideCircle);
		objects.push_back(&HideCircle);
		renderEngine->assignBuffers(EdgeDot);
		renderEngine->updateBuffers(EdgeDot);
		objects.push_back(&EdgeDot);
		renderEngine->assignBuffers(OuterCircle);
		renderEngine->updateBuffers(OuterCircle);
		objects.push_back(&OuterCircle);
		renderEngine->assignBuffers(InnerCircle);
		renderEngine->updateBuffers(InnerCircle);
		objects.push_back(&InnerCircle);
		renderEngine->assignBuffers(MiddleDot);
		renderEngine->updateBuffers(MiddleDot);
		objects.push_back(&MiddleDot);
		renderEngine->assignBuffers(Radius);
		renderEngine->updateBuffers(Radius);
		objects.push_back(&Radius);


		// Render Hypocycloid
		renderEngine->assignBuffers(Hypocycloid);
		renderEngine->updateBuffers(Hypocycloid);
		objects.push_back(&Hypocycloid);

		renderEngine->render(objects, glm::mat4(1.f));
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
