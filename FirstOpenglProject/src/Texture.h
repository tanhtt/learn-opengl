#pragma once

#include "Renderer.h"

class Texture {
private:
	unsigned int m_RendererId;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

};
