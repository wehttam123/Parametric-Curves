#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "Geometry.h"
#include "InputHandler.h"
#include "RenderEngine.h"

class Program {

public:
	Program();
	void start();

	static int width;
	static int height;

	static bool hide;
	static double speed;
	static float r;
	static float R;
	static float n;
	static float angle;
	static float scale;
	static bool rPressed;
	static bool nPressed;

private:
	GLFWwindow* window;
	RenderEngine* renderEngine;

	std::vector<Geometry*> objects;

	Geometry Hypocycloid;
	Geometry OuterCircle;
	Geometry InnerCircle;
	Geometry MiddleDot;
	Geometry EdgeDot;
	Geometry Radius;
	Geometry HideCircle;

	static void error(int error, const char* description);
	void setupWindow();
	void mainLoop();
};