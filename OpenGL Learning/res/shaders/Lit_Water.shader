#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform float u_Time;

vec4 permute(vec4 x);
vec2 fade(vec2 t);
float cnoise(vec2 P);

void main()
{
	vec4 newPos = vec4(position.x, position.y, cnoise(position.xy * sin(u_Time)), 1.0f);

	FragPos = vec3(u_Model * newPos);
	Normal = mat3(transpose(inverse(u_Model))) * normal;
	TexCoords = texCoords;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
};
vec4 permute(vec4 x) { return mod(((x * 34.0) + 1.0) * x, 289.0); }
vec2 fade(vec2 t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }

float cnoise(vec2 P) {
	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
	Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation
	vec4 ix = Pi.xzxz;
	vec4 iy = Pi.yyww;
	vec4 fx = Pf.xzxz;
	vec4 fy = Pf.yyww;
	vec4 i = permute(permute(ix) + iy);
	vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...
	vec4 gy = abs(gx) - 0.5;
	vec4 tx = floor(gx + 0.5);
	gx = gx - tx;
	vec2 g00 = vec2(gx.x, gy.x);
	vec2 g10 = vec2(gx.y, gy.y);
	vec2 g01 = vec2(gx.z, gy.z);
	vec2 g11 = vec2(gx.w, gy.w);
	vec4 norm = 1.79284291400159 - 0.85373472095314 *
		vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
	g00 *= norm.x;
	g01 *= norm.y;
	g10 *= norm.z;
	g11 *= norm.w;
	float n00 = dot(g00, vec2(fx.x, fy.x));
	float n10 = dot(g10, vec2(fx.y, fy.y));
	float n01 = dot(g01, vec2(fx.z, fy.z));
	float n11 = dot(g11, vec2(fx.w, fy.w));
	vec2 fade_xy = fade(Pf.xy);
	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
	return 2.3 * n_xy;
}

#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	int useDiffuseMap;
	sampler2D diffuseMap;
	int useSpecularMap;
	sampler2D specularMap;
	int useNormalMap;
	sampler2D normalMap;

	float shininess;
};
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

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

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform DirLight u_DirLight;
uniform SpotLight u_SpotLights[SPOT_LIGHTS_NUM];
uniform PointLight u_PointLights[POINT_LIGHTS_NUM];
uniform float transparency;

uniform int u_PointLightsCount;
uniform int u_SpotLightsCount;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(u_Material.useNormalMap * vec3(texture(u_Material.normalMap, TexCoords)) - (u_Material.useNormalMap - 1) * Normal);
	//vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	// phase 1: Directional lighting
	vec3 result = CalcDirLight(u_DirLight, norm, viewDir);

	// phase 2: Point lights
	for (int i = 0; i < u_PointLightsCount; i++)
		result += CalcPointLight(u_PointLights[i], norm, FragPos, viewDir);
	// phase 3: Spot lights
	for (int i = 0; i < u_SpotLightsCount; i++)
		result += CalcSpotLight(u_SpotLights[i], norm, FragPos, viewDir);


	color = vec4(result, transparency);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	// combine results
	vec3 ambient = light.ambient * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.ambient);
	vec3 diffuse = light.diffuse * diff * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.diffuse);
	vec3 specular = light.specular * spec * (u_Material.useSpecularMap * vec3(texture(u_Material.specularMap, TexCoords)) - (u_Material.useSpecularMap - 1) * u_Material.specular);

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.ambient);
	vec3 diffuse = light.diffuse * diff * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.diffuse);
	vec3 specular = light.specular * spec * (u_Material.useSpecularMap * vec3(texture(u_Material.specularMap, TexCoords)) - (u_Material.useSpecularMap - 1) * u_Material.specular);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient = light.ambient * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.ambient);
	vec3 diffuse = light.diffuse * diff * (u_Material.useDiffuseMap * vec3(texture(u_Material.diffuseMap, TexCoords)) - (u_Material.useDiffuseMap - 1) * u_Material.diffuse);
	vec3 specular = light.specular * spec * (u_Material.useSpecularMap * vec3(texture(u_Material.specularMap, TexCoords)) - (u_Material.useSpecularMap - 1) * u_Material.specular);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}