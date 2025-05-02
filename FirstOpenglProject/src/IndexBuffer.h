#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID;
	// Size means bytes, if 6 elements, it mean 6 * 4 bytes size
	// Count mean element, if 6 elements mean 6
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	// const means "this function promises not to modify the object", It won't change any member variables
	void Bind() const;
	void Unbind() const;

	// inline means "feel free to optimize this by inserting the code directly."
	inline unsigned int GetCount() const { return m_Count; }
};
