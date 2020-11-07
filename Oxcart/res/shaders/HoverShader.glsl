#shader vertex
#version 330 core
layout (location = 0) in vec2 aVertex;

uniform mat4 ProjectionMatrix;

void main()
{
	gl_Position = ProjectionMatrix * vec4(aVertex.xy, 1.0f, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0, 0, 0, 0.5);
}