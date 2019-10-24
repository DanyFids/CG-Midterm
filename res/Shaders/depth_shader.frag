#version 450 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPos);

	float depth = lightDistance / farPlane;

	gl_FragDepth = depth;
};