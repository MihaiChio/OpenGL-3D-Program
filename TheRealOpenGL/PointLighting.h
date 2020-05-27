#pragma once
#include "lighting.h"
class PointLighting :
	public lighting
{
public:
	PointLighting();
	PointLighting(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat dIntensity, 
					GLfloat xPos, GLfloat yPos, GLfloat zPos,
					GLfloat cont, GLfloat lin, GLfloat exp );

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint expLocation);


	~PointLighting();
protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent; // we will use this to calculate the atenuation of the light. we will use the quadratic formula.
										//ax^2 + bx + c 
};

