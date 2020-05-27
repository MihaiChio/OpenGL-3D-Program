#include "Camera.h"


Camera::Camera() {};

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUpDirection, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat SturnSpeed)
{
	position = startPosition;
	worldUp = startUpDirection;
	yaw = startYaw;
	pitch = startPitch;
	frontPos = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = startMoveSpeed;
	turnSpeed = SturnSpeed;

	update();

}

Camera::~Camera()
{

}

void Camera::keyControl(bool* keys, GLfloat deltaTme)
{
	GLfloat velocity = movementSpeed * deltaTme;
	 
	printf((char*)keys);

	if (keys[GLFW_KEY_W])
	{
		position += frontPos * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= frontPos * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= rightPos * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += rightPos * velocity;
	}
}
void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) // 90 is where the camera will start going behind the character.
	{
		pitch = 89.0f; 
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	update();
}
glm::vec3 Camera::getCameraPos()
{
	return position;
}
glm::vec3 Camera::getCameraDir()
{
	return glm::normalize(frontPos);
}
glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + frontPos, upPos);
}
//only call update when changing angles.

void Camera::update()
{
	frontPos.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontPos.y = sin(glm::radians(pitch));
	frontPos.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontPos = glm::normalize(frontPos); //removing the magnitute, I will define a magnitute of always one.

	rightPos = glm::normalize(glm::cross(frontPos, worldUp)); // we do this to know where the front of our camera is compared to they up of the world.
	upPos = glm::normalize(glm::cross(rightPos, frontPos)); // calculating the camera's UP position.
}
