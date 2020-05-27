#include "textureClass.h"

textureClass::textureClass()
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

textureClass::textureClass(const char* fileLoc)
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool textureClass::loadTexture(bool alpha)
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeats the texture on the shape if the limit is reached S = X axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // repeats the texture on the shape if the limit is reached T = Y Axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // this is done to make the pixels blend together. to make it look more smooth.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (alpha == false)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	}
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); // unbinding the texture.

	stbi_image_free(texData); // freeing the texData.;

	return true;
}
bool textureClass::loadTextureA()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeats the texture on the shape if the limit is reached S = X axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // repeats the texture on the shape if the limit is reached T = Y Axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // this is done to make the pixels blend together. to make it look more smooth.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); // unbinding the texture.

	stbi_image_free(texData); // freeing the texData.;

	return true;
}

void textureClass::useTexture()
{
	glActiveTexture(GL_TEXTURE0); // using the unit 0, the sampler for the shade will have access to the unit and will acess it through the texture UNIT.
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void textureClass::deleteTexture()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

textureClass::~textureClass()
{
	deleteTexture();
}

