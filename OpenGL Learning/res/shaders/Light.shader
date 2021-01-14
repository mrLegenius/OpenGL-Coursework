#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	FragPos = vec3(u_Model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(u_Model))) * normal;
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);

};

#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;


layout(location = 0) out vec4 color;
uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
	//Ambient
	vec3 ambient = u_Light.ambient * u_Material.ambient;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_Light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

	//Specular
	vec3 viewDir = normalize(u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = u_Light.specular * (spec * u_Material.specular);

	//Resulting Phong lighting model
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
};