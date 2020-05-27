#pragma once
#include "lighting.h"
class DirectionalLight :
	public lighting
{
public:
	DirectionalLight();
	~DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat ambIntensity, GLfloat dIntensity, 
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);


private:
	glm::vec3 direction; // the direction of the light.
	
};

