#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class lighting
{
public:
	lighting();
	lighting(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity,  GLfloat dIntensity);


	~lighting();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity; // how much diffuse can the scene see.
};

