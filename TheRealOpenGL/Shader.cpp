#include "Shader.h"
Shader::Shader()
{
	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;

	numberOfPointLights = 0;
	numberOfSpontLights = 0;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::getModelLocation()
{
	return uniformModel;
}

GLuint Shader::getViewLocation()
{
	return uniformView;
}

GLuint Shader::getAmbientIntensityLocation()
{
	return uniformLightDirection.uniformAmbientIntensity;
}

GLuint Shader::getAmbientColourLocation()
{
	return uniformLightDirection.uniformColour;
}
GLuint Shader::getDiffIntensityLocation()
{
	return uniformLightDirection.uniformDiffIntensity;
}
GLuint Shader::getDirectionLocation()
{
	return uniformLightDirection.uniformDirection;
}
GLuint Shader::getShinenessLocation()
{
	return uniformShineness;
}
GLuint Shader::getSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
GLuint Shader::getCameraPosLoc()
{
	return uniformCameraPosition;
}

void Shader::setDirectionalLight(DirectionalLight* directionalLight)
{
	directionalLight->useLight(uniformLightDirection.uniformAmbientIntensity, uniformLightDirection.uniformColour, 
							uniformLightDirection.uniformDiffIntensity, uniformLightDirection.uniformDirection);
}

void Shader::setPointLight(PointLighting* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}

}

void Shader::setSpotLight(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
	{
		lightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffIntensity, uniformSpotLight[i].uniformPosition,uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::useShader()
{
	glUseProgram(shaderId);
}

void Shader::clearShader()
{
	if (shaderId != 0)
	{
		glDeleteProgram(shaderId);
		shaderId = 0;
	}
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderId = glCreateProgram();
	if (!shaderId)
	{
		printf("Shader program has not been created");
		return;
	}

	addShader(shaderId, vertexCode, GL_VERTEX_SHADER); // what program, which shader and what kind of shader.
	addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error validating the program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model"); // get the model  uniform and store it into uniformModel.
	uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
	uniformLightDirection.uniformColour = glGetUniformLocation(shaderId, "DirectionLight.base.colour");
	uniformLightDirection.uniformAmbientIntensity = glGetUniformLocation(shaderId, "DirectionLight.base.ambientIntensity"); // the . specifies exactly which value to get.
	uniformLightDirection.uniformDirection = glGetUniformLocation(shaderId, "DirectionLight.direction");
	uniformLightDirection.uniformDiffIntensity = glGetUniformLocation(shaderId, "DirectionLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
	uniformShineness = glGetUniformLocation(shaderId, "material.shineness");
	uniformCameraPosition = glGetUniformLocation(shaderId, "eyePos");


	uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");


	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuffer[100] = { '\0' };

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.colour", i); // passing the value into loc buffer. d will be replaced with  i. this is done to go through the uniform location
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuffer);
		 
		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffIntensity = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuffer);
	}
	uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");


	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuffer[100] = { '\0' };

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.colour", i); // passing the value into loc buffer. d will be replaced with  i. this is done to go through the uniform location
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffIntensity = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuffer);

		snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderId, locBuffer);
	}


	glDetachShader(shaderId, *fragmentCode);
	glDetachShader(shaderId, *vertexCode);
	glDeleteShader(*fragmentCode);
	glDeleteShader(*vertexCode);
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // will create an empty shader of our type.

	const GLchar* theCode[1];
	theCode[0] = shaderCode; // stores the fragment code within "theCode"

	GLint codeLenght[1];
	codeLenght[0] = strlen(shaderCode); //gets the shadercode length.

	glShaderSource(theShader, 1, theCode, codeLenght); // switching the source of the above specified object to our code.
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog); // storing the error information in the eLog char.
		printf("Error compiling the shader: '%s'\n", eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	clearShader();
}
