#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgrammSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader 
{
private:
	std::string m_Filepath;
	unsigned int m_RendererId;
	// caching uniform
	std::unordered_map<std::string, int> m_UniformLoactionCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgrammSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShaser);
	int GetUniformLocation(const std::string& name);
};
