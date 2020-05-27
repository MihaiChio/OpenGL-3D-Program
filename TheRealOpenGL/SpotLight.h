#pragma once
#include "PointLighting.h"
class SpotLight :
	public PointLighting
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat cont, GLfloat lin, GLfloat exp, GLfloat edg);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
		GLuint constantLocation, GLuint linearLocation, GLuint expLocation, GLuint edgeLocation);
	void setFlash(glm::vec3 pos, glm::vec3 dir);

	~SpotLight();

private:
	glm::vec3 direction;


	GLfloat lightEdge, procEdge; // this is showing the edge of the light instead of showing it 360 like the pointLight.
};

