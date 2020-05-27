#include "lighting.h"


lighting::lighting()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // this is the amount of colour that we want to show. 
	ambientIntensity = 1.0f; // this will make it bright.
	diffuseIntensity = 0.0f;
}

lighting::lighting(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = ambIntensity;
	diffuseIntensity = dIntensity;
}


lighting::~lighting() {}; 