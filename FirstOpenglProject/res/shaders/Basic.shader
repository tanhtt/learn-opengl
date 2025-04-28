#shader vertex
#version 330

layout (location = 0) in vec3 position;
uniform mat4 model;
void main()
{
	gl_Position = model * vec4(position, 1.0);
}


#shader fragment
#version 330

out vec4 color;
void main()
{
	color = vec4(0.3f, 0.5f, 0.2f, 1.0f);
}