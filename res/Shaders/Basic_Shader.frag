#version 450 core
out vec4 FragColor;
in vec3 fragPos;
in vec3 norm;
in vec2 texCoord;
in vec4 sunFLP;
in vec4 fragLightPos[10];

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shine;
};

struct DirLight{
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	sampler2D depthMap;
};

struct PointLight{
	vec3 position;

	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	samplerCube depthMap;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight sun;
uniform PointLight lights[10];
uniform int num_lights;

float shadow_bias = 0.0001;

float far_plane = 25.0;

float SunShadow(vec4 fragInLightSpace){
	vec3 projCoords = fragInLightSpace.xyz / fragInLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	vec2 texelSize = 1.0 / textureSize(sun.depthMap, 0);

	float shadow = 0.0;
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(sun.depthMap, fragInLightSpace.xy + vec2(x,y) * texelSize).r;
			shadow += projCoords.z - shadow_bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow = shadow/9.0;

	float lightDepth = texture(sun.depthMap, projCoords.xy).r;
	float curdepth = projCoords.z;
	
	shadow = curdepth - shadow_bias > lightDepth ? 1.0 : 0.0;

	return shadow;
}

float ShadowCalc(vec3 fragPos, int light_id){
	vec3 fragToLight = fragPos - lights[light_id].position;
	float closestDepth = texture(lights[light_id].depthMap, fragToLight).r;
	closestDepth *= far_plane;

	float curDepth = length(fragToLight);

	float shadow;

	shadow = (curDepth - shadow_bias) > 40.0 ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	vec3 objColor = vec3(texture(material.diffuse, texCoord));
	vec3 specVal = vec3(texture(material.specular, texCoord));
	vec3 normTex = vec3(texture(material.normal, texCoord));

	vec3 normVal = vec3(0.0);

	//vec3 norm = (normTex * 2) - 1; // reverses the normalization to RGB process
	//norm = normalize(vert_out.TBN * norm);

	vec3 sunDir = normalize(sun.pos - vec3(0));

	// Ambient Light
	vec3 ambient = sun.ambient * objColor;

	// Diffuse Light

	float diff = max(dot(norm, sunDir), 0.0);
	vec3 diffuse = sun.diffuse * diff * objColor;

	// Specular Light
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 sHalfW = normalize(viewDir + sunDir);
	vec3 reflectDir = reflect(-sunDir, norm);
	
	float spec = pow(max(dot(sHalfW, reflectDir), 0.0), material.shine);
	vec3 specular = spec * sun.specular * specVal;

	float sunShadow = SunShadow(sunFLP);
	
	vec3 sunColor = ambient + (1.0 - sunShadow)*(diffuse + specular);

	// Point Lights
	vec3 pointColor = vec3(0,0,0);

	for(int c = 0; c < num_lights; c++){
		vec3 lightDir = normalize(lights[c].position - fragPos);
		vec3 halfWay = normalize(viewDir + lightDir);

		float distance = length(lights[c].position - fragPos);
		float attenuation = 1.0 / (1.0 + lights[c].linear * distance + lights[c].quadratic * (distance * distance));

		// Ambient Light
		vec3 p_ambient = lights[c].ambient * objColor;

		// Diffuse Light
		float p_diff = max(dot(norm, lightDir), 0.0);
		vec3 p_diffuse = lights[c].diffuse * p_diff * objColor;

		// Specular Light
		vec3 reflectDir = reflect(-lightDir, norm);
	
		float p_spec = pow(max(dot(norm, halfWay), 0.0), material.shine);
		vec3 p_specular = p_spec * lights[c].specular * specVal;

		float shadow = 1.0 - ShadowCalc(fragPos, c);

		pointColor += (p_ambient + shadow * (p_diffuse + p_specular)) * attenuation;
	}

    FragColor = vec4((pointColor + sunColor), 1.0);
	//FragColor = vec4(objColor, 1.0);
}