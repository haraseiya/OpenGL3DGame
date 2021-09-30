#pragma once
#include <vector>

class Texture;

class GBuffer
{
public:
	// Gバッファに格納されるデータの種類
	enum Type
	{
		Diffuse=0,
		Normal,
		WorldPos,
		Num_GBuffer_Texture
	};

	GBuffer();
	~GBuffer();

	// Gバッファの生成と破棄
	bool Create(int width,int height);
	void Destroy();

	// 指定するテクスチャを取得
	Texture* GetTexture(Type type);

	// フレームバッファオブジェクトのIDを取得
	unsigned int GetBufferID() const { return mBufferID; }

private:
	std::vector<Texture*> mTextures;	// Gバッファに割り当てられたテクスチャ
	unsigned int mBufferID;				// フレームバッファオブジェクトID
};

