#shader vertex
#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;
out vec2 v_TexCoord;
uniform mat4 u_MVP;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = u_MVP * position;
}


#shader fragment
#version 330

layout (location = 0) out vec4 color;
uniform vec4 u_Color;
uniform sampler2D texture1;
uniform sampler2D texture2;
in vec2 v_TexCoord;

void main()
{
	vec4 texColor = mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.4);
	color = texColor * u_Color;
}