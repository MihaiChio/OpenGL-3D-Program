#include "materialClass.h"

materialClass::materialClass()
{
	specularIntensity = 0;
	shininess = 0;
}

materialClass::materialClass(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void materialClass::useMaterial(GLuint specularIntenistyLocation, GLuint shinenessLocation)
{
	glUniform1f(specularIntenistyLocation, specularIntensity);
	glUniform1f(shinenessLocation, shininess);

}
materialClass::~materialClass()
{

}
