#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUpDirection, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat turnSpeed);
	~Camera();

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange); // the mouse does not require delta time.

	glm::vec3 getCameraPos();
	glm::vec3 getCameraDir();

	glm::mat4 calculateViewMatrix();

private:
	glm::vec3 position; //camera position 
	glm::vec3 frontPos; // front  use this as direction 
	glm::vec3 upPos; // up down 
	glm::vec3 rightPos; // X axis.
	glm::vec3 worldUp; // world up position.

	GLfloat yaw; // looking left/right
	GLfloat pitch; // looking up/down

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();

};

