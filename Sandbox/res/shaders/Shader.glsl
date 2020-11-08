#shader vertex
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec4 myVertexColor;
out vec2 myTextureCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0f);
	myVertexColor = aColor;
	myTextureCoord = aTextureCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 myVertexColor;
in vec2 myTextureCoord;

uniform sampler2D uniformTexture0;
uniform sampler2D uniformTexture1;

uniform vec3 uniformObjectColor;
uniform vec3 uniformLightColor;

void main()
{
	//FragColor = mix(texture(uniformTexture0, myTextureCoord), texture(uniformTexture1, myTextureCoord), 0.4);
	FragColor = vec4(uniformLightColor * uniformObjectColor, 1.0);
}