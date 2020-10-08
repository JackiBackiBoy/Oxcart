#shader vertex
#version 330 core
layout (location = 0) in vec4 aVertex;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aVertex.xy, 0.0, 1.0);
	TexCoords = aVertex.zw;
}

#shader fragment
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, TexCoords);
}