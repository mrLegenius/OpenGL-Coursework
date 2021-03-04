#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform float u_Tiling;
uniform vec4 u_Plane;

void main()
{
	gl_ClipDistance[0] = dot(vec4(position, 1.0), u_Plane);
	//u_Projection * u_View * u_Model * 
	gl_Position = vec4(position / 2 + 0.5, 1.0f);
	TexCoords = texCoord * u_Tiling;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{
	float depthValue = texture(u_Texture, TexCoords).r;
	color = vec4(texture(u_Texture, TexCoords).rgb, 1.0);
	
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = texColor;
};