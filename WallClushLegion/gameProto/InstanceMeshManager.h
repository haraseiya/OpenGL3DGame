#pragma once

#include "Math.h"
#include <vector>

class Mesh;
class Shader;
class Texture;
class GameObject;
class InstanceMeshComponent;
class InstanceBase;

class InstanceMeshManager
{
public:
	InstanceMeshManager(InstanceBase* instance,unsigned int maxInstance);
	~InstanceMeshManager();
	
	// インスタンスメッシュをセット
	void SetInstanceMesh();

	// 行列バッファを準備
	void PreparationVAO();

	// シェーダーのセット
	void SetShader();

private:
	// インスタンス群
	//struct Instance
	//{
	//	std::vector<InstanceMeshComponent*> mInstanceMeshComp;
	//	Shader* mInstanceShader;			// シェーダー
	//	Mesh* mMesh;						// メッシュ
	//	Texture* mTexture;					// テクスチャ

	//	unsigned int mVAO;					// VAO	
	//	unsigned int mMaxActorNum;
	//	unsigned int mInstanceVAO;
	//	unsigned int mMeshVAO;
	//	unsigned int mMeshIndicesNum;
	//};

	std::vector<InstanceBase*> mInstances;

	//InstanceBase::InstanceType mInstanceType;
	unsigned int mVAO;
	unsigned int mIndexNum;
	unsigned int mInstanceVAO;					// インスタンスVAO
	unsigned int mMaxInstance;							// 最大インスタンス数
	static const unsigned int mMarixElemNum;
	static const size_t mMatrix4Size;			// 行列の大きさ

	const int mMatRowNum;
	const int mMatColorNum;
	const int mStartAttrib;
	const int mInstanceTypeNum;

	float* mBufferMatrices;
	unsigned int mBuffer;
	//std::vector<GameObject*> mGameObject;
	Texture* mTexture;
	Shader* mInstanceShader;
	Mesh* mMesh;
};

