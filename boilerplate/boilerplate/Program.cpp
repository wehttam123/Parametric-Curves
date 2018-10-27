#define _USE_MATH_DEFINES

#include "Program.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Program::width;
int Program::height;

//bool Program::hide;
//double Program::speed;
//float Program::r;
//float Program::R;
//float Program::n;
//float Program::angle;
//bool Program::rPressed;
////bool Program::nPressed;
double Program::u;	// Parameter
int Program::k;		// Order
int Program::m;		// # of Control Points - 1

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

	//Program::rPressed = false;
	//Program::nPressed = false;

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
	window = glfwCreateWindow(Program::width, Program::height, "CPSC 589 Assignment #2 Parametric Curves", NULL, NULL);
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

	//Program::hide = false;

	Program::u = 0.0;	// Parameter
	Program::k = 0;		// Order
	Program::m = 0;		// # of Control Points - 1

	std::vector<glm::vec2> E;			//Control Values
	std::vector<int> U;					//Knot Values
	std::vector<Geometry> P;			//Control Points
	std::vector<Geometry> G;			//Geometry Objects
	std::vector<glm::vec2> GP;			//Geometry Points

	E.clear();
	U.clear();

	E.push_back(glm::vec2(-4, 0));
	E.push_back(glm::vec2(-4, 4));
	E.push_back(glm::vec2(4, 4));
	E.push_back(glm::vec2(4, 0));

	Program::k = 4;
	Program::m = 3;

	for (int i = 0; i <= (m + k + 1); i++) {
		if (i > 3) {
			U.push_back(1);
		}
		else {
			U.push_back(0);
		}
	}

	std::vector<glm::vec2> C(E.size());	//Vertices

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glLineWidth(3);

		// Hide
		//HideCircle = Geometry::makeCircle(100.0, 0.0, 0.0, 1.0, 1.0, 1.0, Program::hide);

		//Program::k = 4;
		//Program::m = 3;

		//E.clear();
		//U.clear();

		//for (int i = 0; i <= m; i++) {
			//E.push_back(glm::vec2(-4, 0));
			//E.push_back(glm::vec2(-4, 4));
			//E.push_back(glm::vec2(4, 4));
			//E.push_back(glm::vec2(4, 0));
		//}

		//for (int i = 0; i <= m+k+1; i++) {
		//	if (i > 3) {
		//		U.push_back(1);
		//	}
		//	else {
		//		U.push_back(0);
		//	}
		//}

		// Build B-spline
		Spline = Geometry::makeBspline(Program::k, Program::m, E, U, C);

		/*** Calculations for Additional objects in scene ***/

		//Geometry
		Program::u = Program::u += 0.01;
		if (Program::u > 1.0) { Program::u = 0.0; }

		float x = 0.0;
		float y = 0.0;

		G.clear();
		P.clear();
		GP.clear();

		for (int i = 0; i <= (k - 1); i++) {

			C.at(i).x = E.at(Geometry::delta(m, k, u, U) - i).x;
			C.at(i).y = E.at(Geometry::delta(m, k, u, U) - i).y;
		}

		float omega;

		for (int r = k; r >= 2; r--) {
			int i = Geometry::delta(m, k, u, U);
			for (int s = 0; s <= (r - 2); s++) {
				omega = (u - (float)(U.at(i))) / ((float)(U.at(i + r - 1)) - (float)(U.at(i)));
				x = (omega * C.at(s).x) + ((1 - omega) * C.at(s + 1).x);
				y = (omega * C.at(s).y) + ((1 - omega) * C.at(s + 1).y);

				P.push_back(Geometry::makeCircle(0.15, x, y, 0.5, 0.5, 0.5, true));

				GP.push_back(glm::vec2(x, y));

				C.at(s).x = x;
				C.at(s).y = y;
				i = i - 1;
			}

			for (int i = 0; i < GP.size(); i++) {
				if (i > 0) {
					G.push_back(Geometry::makeLine(GP.at(i).x, GP.at(i).y, GP.at(i - 1).x, GP.at(i - 1).y));
				}
			}
			GP.clear();
		}

		for (int i = 0; i < E.size(); i++) {
			P.push_back(Geometry::makeCircle(0.2, E.at(i).x, E.at(i).y, 0.0, 0.0, 0.0, true));
			GP.push_back(glm::vec2(E.at(i).x, E.at(i).y));
		}

		for (int i = 0; i < GP.size(); i++) {
			if (i > 0) {
				G.push_back(Geometry::makeLine(GP.at(i).x, GP.at(i).y, GP.at(i-1).x, GP.at(i-1).y));
			}
		}

		for (int i = 0; i < P.size(); i++) {
			renderEngine->assignBuffers(P.at(i));
			renderEngine->updateBuffers(P.at(i));
			objects.push_back(&P.at(i));
		}

		for (int i = 0; i < G.size(); i++) {
			renderEngine->assignBuffers(G.at(i));
			renderEngine->updateBuffers(G.at(i));
			objects.push_back(&G.at(i));
		}

		//Render Spline
		renderEngine->assignBuffers(Spline);
		renderEngine->updateBuffers(Spline);
		objects.push_back(&Spline);

		renderEngine->render(objects, glm::mat4(1.f));
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
