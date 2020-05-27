#include "Mesh.h"
#include "bugl.h"
Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	indexBufferObject = 0;
	indexCount = 0;
}

void Mesh::createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;


	glGenVertexArrays(1, &VAO); // working on the "VAO" vao.
	glBindVertexArray(VAO);

	glGenBuffers(1, &indexBufferObject); // working with the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject); // element_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO); // generates a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds it so we work on this buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW); // populating the buffer.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // tells openGL how to use the data that we've given it.
													//this will handle the first vertices position with the u and v coordinates.
	glEnableVertexAttribArray(0); // we're enabling the 0 location.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3)); // tells openGL how to use the data that we've given it.
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);// reseting the state.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}
///
void Mesh::renderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // drwaing using the indices instead of the position array.

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // always unbind the element buffer after the vao
}
///
void Mesh::clearMesh()
{
	if (indexBufferObject != 0)
	{
		glDeleteBuffers(1, &indexBufferObject);
		indexBufferObject = 0;  // we do this because there is no garbage collection.
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;  
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;  
	}
	indexCount = 0;
}
 Mesh::~Mesh()
{
	 clearMesh();
}