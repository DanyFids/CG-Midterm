#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 uv;

out vec2 outUV;

uniform mat4 proj;
uniform mat4 model;

void main(){
	
	gl_Position =  proj * model * vec4(pos, 1.0);

	outUV = uv;
}

