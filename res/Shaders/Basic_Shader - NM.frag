#version 450 core
out vec4 FragColor;
in VERT_OUT{
	vec3 fragPos;
	vec2 texCoord;
	mat3 TBN;
} vert_out;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shine;
};

struct DirLight{
	vec3 dir;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight sun;
uniform PointLight lights[10];
uniform int num_lights;

void main()
{
	vec3 objColor = vec3(texture(material.diffuse, vert_out.texCoord));
	vec3 specVal = vec3(texture(material.specular, vert_out.texCoord));
	vec3 normTex = vec3(texture(material.normal, vert_out.texCoord));

	//vec3 normVal = vec3(0.0);
	//
	vec3 norm = (normTex * 2) - 1; // reverses the normalization to RGB process
	norm = normalize(vert_out.TBN * norm);
	//
	//// Ambient Light
	//vec3 ambient = sun.ambient * objColor;
	//
	//// Diffuse Light
	//
	//float diff = max(dot(norm, sun.dir), 0.0);
	//vec3 diffuse = sun.diffuse * diff * objColor;
	//
	//// Specular Light
	//vec3 viewDir = normalize(viewPos - vert_out.fragPos);
	//vec3 reflectDir = reflect(-sun.dir, norm);
	//
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	//vec3 specular = spec * sun.specular * specVal;
	//
	////FragColor = vec4(normalize(lightPos), 1.0);
	//
	//vec3 sunColor = ambient + diffuse + specular;

	// Point Lights
	vec3 pointColor = vec3(0,0,0);

	for(int c = 0; c < num_lights; c++){
		vec3 lightDir = normalize(lights[c].position - vert_out.fragPos);
		float distance = length(lights[c].position - vert_out.fragPos);
		float attenuation = 1.0 / (1.0 + lights[c].linear * distance + lights[c].quadratic * (distance * distance));

		// Ambient Light
		vec3 p_ambient = lights[c].ambient * objColor;

		// Diffuse Light
		float p_diff = max(dot(norm, lightDir), 0.0);
		vec3 p_diffuse = lights[c].diffuse * p_diff * objColor;

		// Specular Light
		vec3 viewDir = normalize(viewPos - vert_out.fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
	
		float p_spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
		vec3 p_specular = p_spec * lights[c].specular * specVal;

		pointColor += (p_ambient + p_diffuse + p_specular) * attenuation;
	}

    FragColor = vec4((pointColor), 1.0);
	//FragColor = vec4(objColor, 1.0);
}