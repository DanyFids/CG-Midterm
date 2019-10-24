#pragma once
#include<string>

class Material {
public:
	unsigned int DIFF;
	unsigned int SPEC;
	unsigned int NORM;
	float shine;

	std::string diff_file;
	std::string spec_file;
	std::string norm_file;

	Material(std::string d, std::string n = "default-normal.png", std::string s = "default-texture.png");
};