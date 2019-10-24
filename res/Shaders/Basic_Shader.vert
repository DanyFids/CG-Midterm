#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 aTan;
layout (location = 4) in vec3 bTan;

out vec3 fragPos;
out vec3 norm;
out vec2 texCoord;
out vec4 sunFLP;
out vec4 fragLightPos[10];

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 corr;

uniform mat4 sunLSM;
uniform mat4 lightSpaceMatrix[10];
uniform int num_lights;

uniform mat3 normMat;

void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0)); 
	texCoord = iTexCoord;

	vec3 t = normalize(normMat * aTan);
	vec3 b = normalize(normMat * bTan);
	vec3 n = normalize(normMat * aNorm);

	norm = n;

	for(int l = 0; l < num_lights; l++){
		fragLightPos[l] =  corr * lightSpaceMatrix[l] * vec4(fragPos, 1); 
	}
	sunFLP = corr * sunLSM * vec4(fragPos, 1);

    gl_Position = corr * project * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}