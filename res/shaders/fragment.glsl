
#version 330 core
layout(location = 0) out vec4 FragColor;

struct Material {
	vec4 color;
};

uniform Material material;

void main()
{
    FragColor = material.color;
}
