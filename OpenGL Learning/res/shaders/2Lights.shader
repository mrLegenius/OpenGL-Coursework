#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	FragPos = vec3(u_Model * position);
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
	Normal = mat3(transpose(inverse(u_Model))) * normal;
	TexCoords = texCoords;
};

#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

layout(location = 0) out vec4 color;

#define NR_POINT_LIGHTS 2

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	vec3 result;
	// phase 2: Point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(u_PointLights[i], norm, FragPos, viewDir);

	//result += CalcPointLight(u_PointLights[0], norm, FragPos, viewDir);
	//result += CalcPointLight(u_PointLights[1], norm, FragPos, viewDir);
	color = vec4(result, 1.0);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Ambient
	vec3 ambient = light.ambient * u_Material.ambient;

	//Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * u_Material.diffuse);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = light.specular * (spec * u_Material.specular);

	//Resulting Phong lighting model
	return ambient + diffuse + specular;
}
