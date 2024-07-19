#pragma once
#include <iostream>

#include <glm/glm.hpp>

class FrameBuffer
{
public:
	void Init(float width, float height);
	~FrameBuffer();
	unsigned int getFrameTexture();
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;
};
