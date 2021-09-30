#include "GBuffer.h"

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
	mTextures.clear();
}

bool GBuffer::Create(int width, int height)
{
	return false;
}

void GBuffer::Destroy()
{
}

Texture* GBuffer::GetTexture(Type type)
{
	return nullptr;
}
