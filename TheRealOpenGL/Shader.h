#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>

#include "commonValuesToBeUsed.h"

#include "DirectionalLight.h"
#include "PointLighting.h"
#include "SpotLight.h"

#include<GL/glew.h>

class Shader
{
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);

	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getShinenessLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getCameraPosLoc();

	void setDirectionalLight(DirectionalLight *directionalLight);
	void setPointLight(PointLighting* pLight, unsigned int lightCount);
	void setSpotLight(SpotLight* sLight, unsigned int lightCount);


	void useShader();
	void clearShader();
	~Shader();

private:

	int numberOfPointLights;
	int numberOfSpontLights;


	GLuint shaderId, uniformProjection, uniformModel, uniformView,
		uniformCameraPosition, uniformSpecularIntensity, uniformShineness;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffIntensity;

		GLuint uniformDirection;
	} uniformLightDirection;

	GLuint uniformPointLightCount;
	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];


	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};


