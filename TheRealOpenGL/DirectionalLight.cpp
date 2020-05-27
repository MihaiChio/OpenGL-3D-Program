#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : lighting() // calls the super class constructor
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat ambIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : lighting(red, green, blue, ambIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColorLocation, colour.x, colour.y, colour.z); // setting up a uniform value that will take in 3 floats.
				//ambient color location stores RGB.
	glUniform1f(ambientIntensityLocation, ambientIntensity); // this will store the intensity in the location.

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

}
DirectionalLight::~DirectionalLight() {}
