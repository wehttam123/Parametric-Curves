#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

class Geometry {

public:
	Geometry();

	GLuint drawMode;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint colourBuffer;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> colours;
	glm::mat4 modelMatrix;

	static Geometry makeCircle(float radius, float x, float y, float r, float g, float b, bool fill);
	static Geometry makeHypocycloid(float r, float R, double n, float angle);
	static Geometry makeLine(float x, float y, float x1, float y1);
};
