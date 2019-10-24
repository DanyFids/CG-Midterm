#version 450 core
layout (location = 0) in vec3 pos;

out vec2 texCoord;

void main(){
	texCoord = (pos.xy + 0.5) * 2;
	gl_Position = vec4(pos, 1);
}