#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initiliase();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeigh() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); } // will be used to loop until the window has been closed in a controlled manner
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();


	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // covering the ASCII character

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;// store the difference between current coords to the last coords.
	GLfloat yChange;
	bool mouseFirstMoved; 


	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // using static because a normal member function will not work with the callback.
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	void createCallBacks();
};// action represents the state of the button, e.g. press.

