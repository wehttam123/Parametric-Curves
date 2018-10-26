#define _USE_MATH_DEFINES
#include "Geometry.h"

#include <cmath>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	modelMatrix = glm::mat4(1.f);
}

Geometry Geometry::makeCircle(float radius, float x1, float y1, float r, float g, float b, bool fill) {

	float x = 0.0;
	float y = 0.0;

	Geometry circle;

	for (double u = 0.0; u <= 2.0 * M_PI; u += 0.01) {
		x = radius * cos(u);
		y = radius * sin(u);

		circle.verts.push_back(glm::vec3(x + x1, y + y1, 0));
		circle.colours.push_back(glm::vec3(r, g, b));

	}

	if (fill) {
		circle.drawMode = GL_TRIANGLE_FAN;
	}
	else {
		circle.drawMode = GL_LINE_LOOP;
	}
	return circle;
}

Geometry Geometry::makeHypocycloid(float r, float R, double n, float angle) {

	float x;
	float y;
	float xr;
	float yr;

	Geometry Hypcycloid;

	for (double u = 0.0; u <= 2.0 * M_PI * n; u += 0.05) {

		// Parametric Equations
		x = ((R - r) * cos(u)) + (r * cos(((R - r) / r) * u));
		y = ((R - r) * sin(u)) - (r * sin(((R - r) / r) * u));

		// Adjust rotation
		xr = (x * cos(angle)) - (y * sin(angle));
		yr = (y * cos(angle)) + (x * sin(angle));

		Hypcycloid.verts.push_back(glm::vec3(xr, yr, 0));
		Hypcycloid.colours.push_back(glm::vec3(1.f, 0.f, 0.f));
	}

	Hypcycloid.drawMode = GL_LINE_STRIP;
	return Hypcycloid;
}

Geometry Geometry::makeLine(float x, float y, float x1, float y1) {

	Geometry line;

	line.verts.push_back(glm::vec3(x, y, 0));
	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));
	line.verts.push_back(glm::vec3(x1, y1, 0));
	line.colours.push_back(glm::vec3(0.f, 0.f, 0.f));

	line.drawMode = GL_LINES;

	return line;
}