#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h> // standard io 
#include <string.h>
#include <cmath>
#include <vector>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //will be using the type ptr to pass the value to the shader, glm values aren't fit to be passed as raw value

#include"commonValuesToBeUsed.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "textureClass.h"
#include "DirectionalLight.h"
#include "materialClass.h"
#include "PointLighting.h"
#include "SpotLight.h"
#include "Model.h"

//Helpful for report// DELETE WHEN COMPLETE
/*/ model matrix will allows us to move from model coordinates to world coordinates.




//////////////////////////////////////DELETE WHEN DONE*/
const float toRadian = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> objectList;
std::vector<Shader> shaderList;
GLuint shaderProgram, uniformModel, uniformProjection, uniformView;
Camera mainCamera;



textureClass galaxyTexture;
textureClass sandTexture;
textureClass mudTexture;

materialClass shinyMaterial;
materialClass dullMaterial;

Model silverArrow;


DirectionalLight mainLight;
PointLighting pointLight[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
////////////////Will be removed, used to demonstrate certain features and debug.

//Vertex Shader
const char* vertexShader = "Shaders/shader.vert";

//Fragment Shader
const char* fragmentShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int numOfVertices, unsigned int vLength, unsigned int normalOffset);
void createObjects();
void createShaders();



int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.initiliase();

	//creating objects
	createObjects();
	createShaders();
	

	mainCamera = Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, 0.0f, 5.0f, 0.1f);
	sandTexture = textureClass("Textures/sand.png");
	sandTexture.loadTexture(true); // uses an alpha boolean to represen the alpha channel.
	galaxyTexture = textureClass("Textures/galaxy.jpg");
	galaxyTexture.loadTextureA();
	mudTexture = textureClass("Textures/mud.jpg");
	mudTexture.loadTextureA();

	shinyMaterial = materialClass(1.0f, 32); // 32 common value for average shiny object.
	dullMaterial = materialClass(0.3f, 4);

	silverArrow = Model();
	silverArrow.loadModel("Models/falc_low.obj");


	mainLight = DirectionalLight(1.0f,1.0f,1.0f,
									0.2f, 0.2f,
								0.0f,0.0f,0.0f); // direction and intensitiy of the light
			//X light to the right/ Y -1,0 = down / Z = -2.0 / 1.0f Intensity.


	unsigned int numPointLights = 0;

	pointLight[0] = PointLighting(0.0f, 0.0f,0.0f,
								 0.5f, 0.4f,
								 -4.0f, 0.0f, 0.0f,
								 0.3f, 0.2f, 0.1f); 


	numPointLights++;

	pointLight[1] = PointLighting(0.3f,0.3f,0.3f,
								0.1f, 0.4f,
								4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);

	numPointLights++;

	//creating spotlights
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(0.8f, 0.8f, 0.8f,
		0.3f, 0.6f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
		spotLightCount++;

	spotLights[1] = SpotLight(0.8f, 0.8f, 0.8f,
		0.4f, 1.0f,
		2.0f, -1.5f, -0.5f,
		-5.0f, -1.0f, +1.15f,
		1.0f, 0.0f, 0.0f,
		20.0f);
		spotLightCount++;


	// Creating the projection
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShineness = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeigh(),0.1f,100.0f); // for not it doesn't need to be changed.

	//Loop until window closes
	while (!mainWindow.getShouldClose())
	{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// get+handel user input event.

		mainCamera.keyControl(mainWindow.getKeys(), deltaTime);
		mainCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Clear Window
		glClearColor(0.0f,0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		shaderList[0].useShader(); // useProgram
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformEyePosition = shaderList[0].getCameraPosLoc();
		uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
		uniformShineness = shaderList[0].getShinenessLocation();

		glm::vec3 lowerLight = mainCamera.getCameraPos();
		lowerLight.y -= 0.3;

		shaderList[0].setDirectionalLight(&mainLight);
		shaderList[0].setPointLight(pointLight,numPointLights);
		shaderList[0].setSpotLight(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, mainCamera.getCameraPos().x, mainCamera.getCameraPos().y, mainCamera.getCameraPos().z); // attaching the camera position to the fragment eye.
		 
		glm::mat4 modelMatrix(1.0f); //automatically set to an identiy matrix

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.5f));	// apply translation to it that will only alter the X value.
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix)); // model is not a raw format that will work with the format thats why value_ptr is used.
		galaxyTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity,uniformShineness);
		objectList[0]->renderMesh();



		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15f, -1.99f, -0.15f));	// apply translation to it that will only alter the X value.
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShineness);
		silverArrow.renderModel();

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, -0.0f));	// apply translation to it that will only alter the X value.
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		mudTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShineness);
		objectList[2]->renderMesh();

			
		glUseProgram(0);
		mainWindow.swapBuffers();
		glfwPollEvents();//checks if any event happened.// keeps swapping between the scenes that you're drawing and the one that's currently visible.
	}


	return 0;
}

void createObjects()
{
	//create Indices
	// indices are telling the GPU which positions that were already declared to draw.
	unsigned int indices[] =
	{
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	//VAO holds multiple VBOs, VBO holds the actual data.
	GLfloat positions[] =
	{
		//x    y	z		   u     v
		-1.0f, -1.0f, -0.6f,   0.0f, 0.0f,   0.0f,0.0f,0.0f,
		0.0f, -1.0f, 1.0f,    0.5f, 0.0f,	  0.0f,0.0f,0.0f,
		1.0f, -1.0f, -0.6f,   1.0f, 1.0f,	  0.0f,0.0f,0.0f,
		0.0f, 1.0f, 0.0f,    0.5,  1.0f,     0.0f,0.0f,0.0f
	};

	unsigned int floorIndices[] =
	{
		//top left - bottom left - top right -bottom left
		0,2,1,
		1,2,3
	};

	GLfloat floorPosition[] =
	{
		-10.0f,0.0f,-10.0f,		0.0f,0.0f,		0.0f, -1.0f, 0.0f,
		10.0f,0.0f,-10.0f,		10.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f,0.0f,10.0f,		0.0f,10.0f,		 0.0f, -1.0f, 0.0,
		10.0f,0.0f,10.0f,		10.0f, 10.0f,      0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, positions, 32, 8, 5);

	Mesh* obj1 = new Mesh(); 
	obj1->createMesh(positions, indices, 32, 12);
	objectList.push_back(obj1);
	Mesh* obj2 = new Mesh();
	obj2->createMesh(positions, indices, 32, 12);
	objectList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->createMesh(floorPosition, floorIndices, 32, 6);
	objectList.push_back(obj3);

}
void createShaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vertexShader, fragmentShader); // loading and applying the functions for the shader
	shaderList.push_back(*shader1); // putting in the vector.
	
}
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat * vertices, unsigned int numOfVertices, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3) // increasing by 3 to jump from triangle to triangle.
	{
		unsigned int in0 = indices[i] * vLength; // this allows us to grab a specific position.
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;


		//create 2 lines and then we'll cross product to find a vector at right angles from both of them.
		glm::vec3 v1(vertices[in1] - vertices[in2], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);

		normal = glm::normalize(normal); // unit vector.

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z; // we're reading the normal here.
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z; // we're reading the normal here.
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z; // we're reading the normal here.

	}

	for (size_t i = 0; i < numOfVertices / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset; // counting the indices and accessing the relevant normal.
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
		// grabbing the start of each normal, normalise them and putting the values back.
	}
}