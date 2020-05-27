#pragma once

#include <GL/glew.h>
#include "stb_image.h"


class textureClass
{
public:
	textureClass();
	textureClass(const char* fileLoc);

	bool loadTexture(bool alpha);
	bool loadTextureA();
	void useTexture();
	void deleteTexture();

	~textureClass();


private:
	GLuint textureId;
	int width, height, bitDepth; 

	const char* fileLocation;
};

