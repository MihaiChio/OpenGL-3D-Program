#include "SpotLight.h"
SpotLight::SpotLight()
{
	direction = glm::vec3(0.0f, -1.0f, -0.0f);
	lightEdge = 0.0f;
	procEdge = cosf(glm::radians(lightEdge));

}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat cont, GLfloat lin, GLfloat exp, GLfloat edg) : PointLighting(red,green,blue,ambIntensity, dIntensity,xPos,yPos,zPos,cont,lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	lightEdge = edg;
	procEdge = cosf(glm::radians(lightEdge));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint expLocation, GLuint edgeLocation)
{
	glUniform3f(ambientColorLocation, colour.x, colour.y, colour.z); // setting up a uniform value that will take in 3 floats.//ambient color location stores RGB.
	glUniform1f(ambientIntensityLocation, ambientIntensity); // this will store the intensity in the location.
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(expLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{

}