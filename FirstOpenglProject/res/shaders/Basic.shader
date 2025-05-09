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
uniform sampler2D u_Texture;
in vec2 v_TexCoord;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor * u_Color;
}