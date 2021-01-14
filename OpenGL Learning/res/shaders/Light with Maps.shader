#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec2 TexCoords;

uniform vec3 u_LightPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	FragPos = vec3(u_View * u_Model * position);
	gl_Position = u_Projection * vec4(FragPos, 1.0);
	Normal = mat3(transpose(inverse(u_View * u_Model))) * normal;
	LightPos = vec3(u_View * vec4(u_LightPos, 1.0));
	TexCoords = texCoords;
};

#shader fragment
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
in vec3 LightPos;
in vec2 TexCoords;

layout(location = 0) out vec4 color;

//uniform vec3 u_Color;
uniform vec3 u_LightColor;

uniform Material u_Material;
uniform Light u_Light;
uniform float u_Time;

void main()
{
	//Ambient
	vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoords).rgb;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoords).rgb;

	//Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specTexture = texture(u_Material.specular, TexCoords).rgb;
	vec3 specular = u_Light.specular * spec * specTexture;

	//Emission
	vec3 emission = vec3(0.0);
	if(specTexture == vec3(0.0))
		emission = texture(u_Material.emission, TexCoords + vec2(0.0, u_Time / 3)).rgb * (sin(u_Time) * 0.5 + 0.5);

	//Resulting Phong lighting model with lighting maps
	color = vec4(ambient + diffuse + specular + emission, 1.0);
};