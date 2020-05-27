#pragma once
#include <GL/glew.h>
#include <stdio.h>

class Mesh
{
public:
	Mesh();
	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numberOfVertices,unsigned int numOfIndices); // create 
	void renderMesh(); // draw
	void clearMesh(); // clear from GPU
	~Mesh();

private:
	GLuint VAO, VBO, indexBufferObject;
	GLsizei indexCount;
};

