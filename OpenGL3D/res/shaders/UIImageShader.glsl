#shader vertex
#version 330 core
layout (location = 0) in vec4 aVertex;
out vec2 TexCoords;

uniform mat4 ProjectionMatrix;

void main()
{
	gl_Position = ProjectionMatrix * vec4(aVertex.xy, 1.0, 1.0);
	TexCoords = aVertex.zw;
}

#shader fragment
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture;
uniform vec4 ColorOverlay;

void main()
{
	FragColor = texture2D(Texture, TexCoords) * ColorOverlay;
}