#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;


struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct directionalLight
{
    Light base;
	vec3 direction;

};


struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
	
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shineness;
};

uniform int pointLightCount;
uniform int spotLightCount;



uniform directionalLight DirectionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePos;

vec4 CalculateByDirection(Light light,vec3 direction)
{
	vec4 ambientColour = vec4(light.colour,1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);
	vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;
	
	vec4 specularColour = vec4 (0,0,0,0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePos - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye,reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor,material.shineness);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		} 
	}
	return (ambientColour + diffuseColour + specularColour);
}


vec4 calculateDirectionalLight()
{
	return CalculateByDirection(DirectionLight.base, DirectionLight.direction);
}


vec4 calcPointLight(PointLight pLight)
{
		vec3 direction = FragPos - pLight.position;
		float distance =length(direction);
		direction = normalize(direction);
		
		vec4 colour = CalculateByDirection(pLight.base,direction);
		float attenuation = pLight.exponent * distance * distance +
							pLight.linear * distance + 
							pLight.constant;
							
		return (colour/attenuation);
}

vec4  CalculateSpotLight (SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position); // angle between the direction and where the cutoff is, this is done to decide wether or not the area should be lit.
	float slFactor = dot(rayDirection, sLight.direction);	//calculate the spot light factor.
	
	if(slFactor > sLight.edge) // is the spotlight factor than the age, if slFactor is 1 it will be perfect with the ray direction
	{
		vec4 colour = calcPointLight(sLight.base); // we want to map slFactor to be between  0 and 1.
		
		
		
		return  colour * (1.0f - (1.0f - slFactor)*((1.0f /(1.0f - sLight.edge))));
	}
	else
	{
	vec4 (0,0,0,0);
	}

}

vec4 CalculatePointLights()
{
	vec4 totalColour = vec4(0,0,0,0);
	
	for(int i = 0; i < pointLightCount; i++)
	{
		
		totalColour += calcPointLight(pointLights[i]);
	}
		return totalColour;
}

vec4 calcSpotLights()
{
	vec4 totalColour = vec4(0,0,0,0);
	
	for(int i = 0; i < spotLightCount; i++)
	{
		
		totalColour += CalculateSpotLight(spotLights[i]);
	}
		return totalColour;
}


void main()
{

	vec4 finalColour = calculateDirectionalLight();
	finalColour += CalculatePointLights();
	finalColour += calcSpotLights();
	
	colour = texture(theTexture,TexCoord) * finalColour;
}