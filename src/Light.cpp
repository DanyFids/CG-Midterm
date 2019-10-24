#include "Light.h"
#include "Shader.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GLM/gtc/matrix_transform.hpp>
#include "Constants.h"

void Light::SetupOccRender()
{
	lightView = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	GLenum err;

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	err = glGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	err = glGetError();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	err = glGetError();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Light::Light(glm::vec3 pos, bool orth)
{
	if (orth) {
		far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.2f, far_plane);
	}
	else {
		far_plane = 25.0f;
		lightProjection = glm::perspective(glm::radians(90.0f), ((float)SHADOW_WIDTH / (float)SHADOW_HEIGHT), 0.2f, far_plane);
	}
	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.2f, 100.0f);

	position = pos;
	ambient = glm::vec3(1.0, 1.0, 1.0);
	diffuse = ambient;
	specular = diffuse;

	ambient_strength = 0.2f;
	diffuse_strength = 0.5f;
	specular_strength = 1.0f;

	SetupOccRender();
}

Light::Light(glm::vec3 pos, glm::vec3 color, float a, float d, float s, bool orth)
{
	if (orth) {
		far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.2f, far_plane);
	}
	else {
		far_plane = 25.0f;
		lightProjection = glm::perspective(glm::radians(90.0f), ((float)SHADOW_WIDTH / (float)SHADOW_HEIGHT), 0.2f, far_plane);
	}

	position = pos;

	ambient = color;
	diffuse = color;
	specular = color;

	ambient_strength = a;
	diffuse_strength = d;
	specular_strength = s;

	SetupOccRender();
}

Light::Light(glm::vec3 pos, glm::vec3 ambi, glm::vec3 diff, glm::vec3 spec, float a, float d, float s, bool orth)
{
	if (orth) {
		far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.2f, far_plane);
	}
	else {
		far_plane = 25.0f;
		lightProjection = glm::perspective(glm::radians(90.0f), ((float)SHADOW_WIDTH / (float)SHADOW_HEIGHT), 0.2f, far_plane);
	}

	position = pos;

	ambient = ambi;
	diffuse = diff;
	specular = spec;

	ambient_strength = a;
	diffuse_strength = d;
	specular_strength = s;

	SetupOccRender();
}

void Light::SetUniformColor(glm::vec3 color)
{
	ambient = diffuse = specular = color;
}

void Light::SetDiffuse(glm::vec3 color, float str)
{
	diffuse = color;
	diffuse_strength = str;
}

void Light::SetSpecular(glm::vec3 color, float str)
{
	specular = color;
	specular_strength = str;
}

void Light::SetStr(float a, float d, float s)
{
	ambient_strength = a;
	diffuse_strength = d;
	specular_strength = s;
}

void Light::SetAmbient(glm::vec3 color, float str)
{
	ambient = color;
	ambient_strength = str;
}

void Light::SetupLight(Shader* shader)
{
	shader->SetVec3("light.position", position);
	shader->SetVec3("light.ambient", (ambient * ambient_strength));
	shader->SetVec3("light.diffuse", (diffuse * diffuse_strength));
	shader->SetVec3("light.specular", (specular * specular_strength));
}

void Light::SetupDepthShader(Shader* shader)
{
	shader->SetMat4( "lightSpaceMatrix", lightProjection * lightView);
}

DirectionalLight::DirectionalLight(glm::vec3 pos):Light(pos){}
DirectionalLight::DirectionalLight(glm::vec3 pos, glm::vec3 color, float a, float d, float s):Light(pos, color, a, d, s){}
DirectionalLight::DirectionalLight(glm::vec3 pos, glm::vec3 ambi, glm::vec3 diff, glm::vec3 spec, float a, float d, float s):Light(pos, ambi, diff, spec, a, d, s){}

void DirectionalLight::SetupLight(Shader* shader)
{
	shader->SetVec3("sun.pos", position);
	shader->SetVec3("sun.ambient", (ambient * ambient_strength));
	shader->SetVec3("sun.diffuse", (diffuse * diffuse_strength));
	shader->SetVec3("sun.specular", (specular * specular_strength));
	shader->SetI("sun.depthMap", 3);

	shader->SetMat4("sunLSM", lightProjection * lightView);
}

void PointLight::SetupCubeMap()
{
	glDeleteTextures(1, &depthMap);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glDeleteFramebuffers(1, &depthMapFBO);
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightTransforms.push_back(glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

PointLight::PointLight(glm::vec3 pos):Light(pos, false)
{
	linear = 0.7f;
	quadratic = 1.8f;

	SetupCubeMap();
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float a, float d, float s, float l, float q): Light(pos, color, a, d,s, false)
{
	linear = l;
	quadratic = q;

	SetupCubeMap();
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 ambi, glm::vec3 diff, glm::vec3 spec, float a, float d, float s, float l, float q):Light(pos, ambi, diff, spec, a, d, s, false)
{
	linear = l;
	quadratic = q;

	SetupCubeMap();
}

void PointLight::SetupLight(Shader* shader)
{
	shader->SetVec3("lights[0].position", position);
	shader->SetVec3("lights[0].ambient", (ambient * ambient_strength));
	shader->SetVec3("lights[0].diffuse", (diffuse * diffuse_strength));
	shader->SetVec3("lights[0].specular", (specular * specular_strength));

	shader->SetF("lights[0].linear", linear);
	shader->SetF("lights[0].quadratic", quadratic);
}

void PointLight::SetupLight(Shader* shader, int num)
{
	std::string name = "lights[" + std::to_string(num) + "]";
	std::string temp;

	shader->SetMat4("lightSpaceMatrix[" + std::to_string(num) + "]", lightProjection * lightView);

	temp = name + ".position";
	shader->SetVec3(temp.c_str(), position);
	temp = name + ".ambient";
	shader->SetVec3(temp.c_str(), (ambient * ambient_strength));
	temp = name + ".diffuse";
	shader->SetVec3(temp.c_str(), (diffuse * diffuse_strength));
	temp = name + ".specular";
	shader->SetVec3(temp.c_str(), (specular * specular_strength));

	temp = name + ".linear";
	shader->SetF(temp.c_str(), linear);
	temp = name + ".quadratic";
	shader->SetF(temp.c_str(), quadratic);

	temp = name + ".depthMap";
	shader->SetI(temp.c_str(), 4 + num);
}

void PointLight::SetupDepthShader(Shader* shader)
{
	for (int c = 0; c < lightTransforms.size(); c++) {
		std::string locName = "shadowMatrix[" + std::to_string(c) + "]";
		shader->SetMat4(locName.c_str(), lightTransforms[c]);
	}
	shader->SetVec3("lightPos", position);
	shader->SetF("farPlane", far_plane);
}
