#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 aTan;
layout (location = 4) in vec3 bTan;

out VERT_OUT{
	vec3 fragPos;
	vec2 texCoord;
	mat3 TBN;
} vert_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 corr;

void main()
{
    gl_Position = corr * project * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vert_out.fragPos = vec3(model * vec4(aPos, 1.0)); 
	vert_out.texCoord = iTexCoord;

	vec3 t = normalize(vec3(model * vec4(aTan, 0.0)));
	vec3 b = normalize(vec3(model * vec4(bTan, 0.0)));
	vec3 n = normalize(vec3(model * vec4(aNorm, 0.0)));

	vert_out.TBN = mat3(t,b,n);
}