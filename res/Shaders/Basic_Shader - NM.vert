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

uniform mat3 normMat;

void main()
{
    gl_Position = corr * project * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vert_out.fragPos = vec3(model * vec4(aPos, 1.0)); 
	vert_out.texCoord = iTexCoord;

	vec3 t = normalize(normMat * aTan);
	vec3 b = normalize(normMat * bTan);
	vec3 n = normalize(normMat * aNorm);

	vert_out.TBN = mat3(t,b,n);
}