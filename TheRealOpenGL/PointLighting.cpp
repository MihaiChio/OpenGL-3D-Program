#include "PointLighting.h"


PointLighting::PointLighting() : lighting()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // initialising the constant to 1 so the program won't crash.
	linear = 0.0f;
	exponent = 0.0f;

	//L/AX^2+BX+C
}

PointLighting::PointLighting(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat ambIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat cont, GLfloat lin, GLfloat exp) : lighting(red, green, blue, ambIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = cont;
	linear = lin;
	exponent = exp;
}

void PointLighting::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint expLocation)
{
	glUniform3f(ambientColorLocation, colour.x, colour.y, colour.z); // setting up a uniform value that will take in 3 floats.//ambient color location stores RGB.
	glUniform1f(ambientIntensityLocation, ambientIntensity); // this will store the intensity in the location.
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(expLocation, exponent);
}

PointLighting::~PointLighting()
{

}
