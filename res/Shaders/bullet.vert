#version 460 core
layout (location = 0) in vec3 aPos;
out vec4 inColor; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 corr;

void main()
{
    gl_Position = corr * project * view * model * vec4(aPos, 1.0);
    inColor = vec4(1.0, 0.8, 0.0, 1.0);
}