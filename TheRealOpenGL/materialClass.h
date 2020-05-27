#pragma once

#include <GL\glew.h>

class materialClass
{
public:
	materialClass();
	materialClass(GLfloat sIntensity, GLfloat shine);
	~materialClass();

	void useMaterial(GLuint specularIntenistyLocation, GLuint shinenessLocation);


private:
	GLfloat specularIntensity; // how much specular light on the object. How much of the light will the object absorb
	GLfloat shininess; // how smooth the surface is gonna be portrayed as. How collected will be on one point.
};

