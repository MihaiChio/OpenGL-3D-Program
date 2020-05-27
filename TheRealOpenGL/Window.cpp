#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0;  i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) // set the w and h values.
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::initiliase()
{
	//intialise GLFW
	if (!glfwInit())
	{
		printf("GFLW Failed");
		glfwTerminate(); // this will close the program.
		return 1; // an error has been returned, the program exited in an uncontrolled manner.
	}

	//setup GLFW window properties.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // setting the major version of the OpenGL that we want to use.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // this is saying that we're using openGL 3.3
	// this will not allow us to use deprecated features. Core PROFILE = no backwards compatiblity.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // allow forward compatibilty.

	 mainWindow = glfwCreateWindow(width, height, "3DGP ASSIGNMENT", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW WINDOW FAILED...");
		glfwTerminate();
		return 1;
	}

	//Get buffer size information.
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // we do this to get the window size in pixels rather than screen coordinates
																	//later on  this will be used for the view port, this will make the objects drawn look the same on different monitors.

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); // make this window the current context.
	//user input
	createCallBacks();

	//Locking the window to the window.
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//allow modern features
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("ERRROR: \n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // this is to check which triangles are deeper into the image so it knows which one it can draw on top.

	//setup viewport size
	glViewport(0, 0, bufferWidth, bufferWidth);

	glfwSetWindowUserPointer(mainWindow, this); //we use to let the callback function access this class.
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // we do this so we can access the window
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos; // setting the last X and Y to the current ones to avoid having them on 0.
		theWindow->mouseFirstMoved = false;
	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // this is done to avoid up and down movement. 

	theWindow->lastX = xPos;
	theWindow->lastY = yPos; 

}

void Window::createCallBacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse); 
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}