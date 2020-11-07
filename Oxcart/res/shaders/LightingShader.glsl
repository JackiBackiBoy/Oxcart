#shader vertex
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 someTexCoords;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	FragPosition = vec3(ModelMatrix * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;
	TexCoords = someTexCoords;

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPosition, 1.0f);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantTerm;
	float linearTerm;
	float quadraticTerm;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

	float constantTerm;
	float linearTerm;
	float quadraticTerm;
};

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

	float constantTerm;
	float linearTerm;
	float quadraticTerm;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoords;

uniform DirectionalLight directionalLight;

uniform Material material;
uniform vec3 ViewPosition;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalculateDirectionalLight(DirectionalLight aDirectionalLight, vec3 aNormal, vec3 aViewDirection);
vec3 CalculatePointLight(PointLight aPointLight, vec3 aNormal, vec3 aFragPosition, vec3 aViewDirection);
vec3 CalculateSpotLight(SpotLight aSpotLight, vec3 aNormal, vec3 aFragPosition, vec3 aViewDirection);

void main()
{
	vec3 tempNormalizedNormal = normalize(Normal);
	vec3 tempNormalizedViewDirection = normalize(ViewPosition - FragPosition);

	// Directional lights
	vec3 tempResult = CalculateDirectionalLight(directionalLight, tempNormalizedNormal, tempNormalizedViewDirection);

	// Point lights
	for (int i = 0; i < 0; i++)
	{
		tempResult += CalculatePointLight(pointLights[i], tempNormalizedNormal, FragPosition, tempNormalizedViewDirection);
	}

	// Spot lights
	tempResult += CalculateSpotLight(spotLight, tempNormalizedNormal, FragPosition, tempNormalizedViewDirection);

	FragColor = vec4(tempResult, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight aDirectionalLight, vec3 aNormal, vec3 aViewDirection)
{
	vec3 tempLightDirection = normalize(-aDirectionalLight.direction);

	// Diffuse
	float tempDiff = max(dot(aNormal, tempLightDirection), 0.0);
	
	// Specular
	vec3 tempReflectionDirection = reflect(-tempLightDirection, aNormal);
	float tempSpec = pow(max(dot(aViewDirection, tempReflectionDirection), 0.0), material.shininess);

	vec3 tempAmbient = aDirectionalLight.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 tempDiffuse = aDirectionalLight.diffuse * tempDiff * vec3(texture(material.diffuse, TexCoords));
	vec3 tempSpecular = aDirectionalLight.specular * tempSpec * vec3(texture(material.specular, TexCoords));

	// Combine diffuse and specular for the result
	return (tempAmbient + tempDiffuse + tempSpecular);
}

vec3 CalculatePointLight(PointLight aPointLight, vec3 aNormal, vec3 aFragPosition, vec3 aViewDirection)
{
	vec3 tempLightDirection = normalize(aPointLight.position - aFragPosition);

	// Diffuse
	float tempDiff = max(dot(aNormal, tempLightDirection), 0.0);

	// Specular
	vec3 tempReflectionDirection = reflect(-tempLightDirection, aNormal);
	float tempSpec = pow(max(dot(aViewDirection, tempReflectionDirection), 0.0), material.shininess);

	// Attenuation
	float tempDistance = length(aPointLight.position - aFragPosition);
	float tempAttenuation = 1.0 / (aPointLight.constantTerm + aPointLight.linearTerm * tempDistance + aPointLight.quadraticTerm * (tempDistance * tempDistance));

	// Combine results
	vec3 tempAmbient = aPointLight.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 tempDiffuse = aPointLight.diffuse * tempDiff * vec3(texture(material.diffuse, TexCoords));
	vec3 tempSpecular = aPointLight.specular * tempSpec * vec3(texture(material.specular, TexCoords));

	tempAmbient *= tempAttenuation;
	tempDiffuse *= tempAttenuation;
	tempSpecular *= tempAttenuation;

	return (tempAmbient + tempDiffuse + tempSpecular);
}

vec3 CalculateSpotLight(SpotLight aSpotLight, vec3 aNormal, vec3 aFragPosition, vec3 aViewDirection)
{
	vec3 tempLightDirection = normalize(aSpotLight.position - aFragPosition);

	// Diffuse
	float tempDiff = max(dot(aNormal, tempLightDirection), 0.0);

	// Specular
	vec3 tempReflectionDirection = reflect(-tempLightDirection, aNormal);
	float tempSpec = pow(max(dot(aViewDirection, tempReflectionDirection), 0.0), material.shininess);

	// Attenuation
	float tempDistance = length(aSpotLight.position - aFragPosition);
	float tempAttenuation = 1.0 / (aSpotLight.constantTerm + aSpotLight.linearTerm * tempDistance + aSpotLight.quadraticTerm * (tempDistance * tempDistance));

	// Spotlight intensity (smooth edges and light cone)
	float tempSpotLightConeAngle = dot(tempLightDirection, normalize(-aSpotLight.direction));
	float tempEpsilon = aSpotLight.cutOff - aSpotLight.outerCutOff;
	float tempIntensity = clamp((tempSpotLightConeAngle - aSpotLight.outerCutOff) / tempEpsilon, 0.0, 1.0);

	// Combine results
	vec3 tempAmbient = aSpotLight.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 tempDiffuse = aSpotLight.diffuse * tempDiff * vec3(texture(material.diffuse, TexCoords));
	vec3 tempSpecular = aSpotLight.specular * tempSpec * vec3(texture(material.specular, TexCoords));

	tempAmbient *= tempAttenuation * tempIntensity;
	tempDiffuse *= tempAttenuation * tempIntensity;
	tempSpecular *= tempAttenuation * tempIntensity;

	return (tempAmbient + tempDiffuse + tempSpecular);
}