#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Texture.h"

Material::Material(std::string f, std::string n, std::string s)
{
	unsigned int texture[3];
	glGenTextures(3, texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(f.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::TEX::ERROR_LOADING_FILE: '" << f  << "'\n" << std::endl;
		unsigned char* miss_data = stbi_load("missing_tex.png", &width, &height, &nrChannels, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, miss_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int nwidth, nheight, nnrChannels;
	unsigned char* ndata = stbi_load(n.c_str(), &nwidth, &nheight, &nnrChannels, 0);

	if (ndata) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nwidth, nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ndata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::NORM::ERROR_LOADING_FILE: '" << n << "' \n" << std::endl;
	}

	stbi_image_free(ndata);

	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int swidth, sheight, snrChannels;
	unsigned char* sdata = stbi_load(s.c_str(), &swidth, &sheight, &snrChannels, 0);

	if (sdata) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, swidth, sheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::SPEC::ERROR_LOADING_FILE: '" << s << "'\n" << std::endl;
	}

	stbi_image_free(sdata);

	diff_file = f;
	norm_file = n;
	spec_file = s;
	DIFF = texture[0];
	NORM = texture[1];
	SPEC = texture[2];

	shine = 32;
}
