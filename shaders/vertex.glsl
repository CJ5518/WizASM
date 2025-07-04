#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

uniform mat4 modelMatrix;
uniform mat4 proj;
uniform mat4 view;

void main()
{
   gl_Position = proj * view * modelMatrix * vec4(aPos, 1.0);
}

