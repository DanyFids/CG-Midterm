#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 uv;

out vec2 outUV;

uniform vec2 SCREEN;
uniform vec2 DIM;
uniform vec2 POS;

void main(){
	float offsetX = 1.0 - DIM.x / SCREEN.x;
	float offsetY = 1.0 - DIM.y / SCREEN.y;

	vec2 pos2D = vec2(pos.x, pos.y) * vec2(DIM.x / SCREEN.x, DIM.y / SCREEN.y) - vec2(offsetX, offsetY) + vec2(POS.x / SCREEN.x, POS.y / POS.y);

	gl_Position =  vec4(pos2D, 0.0, 1.0);

	outUV = uv;
}

