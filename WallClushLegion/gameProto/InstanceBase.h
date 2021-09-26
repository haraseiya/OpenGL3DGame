#pragma once
#include "GameObject.h"

class Mesh;
class Shader;
class Texture;
class MeshComponent;
class InstanceMeshComponent;
class InstanceMeshManager;

class InstanceBase : public GameObject
{
public:
	// インスタンスのタイプ
	enum class InstanceType
	{
		PlayerBullet = 0,	// プレイヤーの弾
		EnergyCapsule,		// エネルギーカプセル

		InstanceTypeNum
	};

	InstanceBase(Tag tag, Mesh* mesh);
	~InstanceBase();

	Mesh* GetMesh() { return mMesh; }

protected:
	InstanceMeshComponent* mInstanceMeshComp;
	InstanceMeshManager* mInstanceMeshManager;
	Shader* mInstanceShader;			// シェーダー
	Mesh* mMesh;						// メッシュ
	MeshComponent* mMeshComp;			// メッシュコンポーネント
	Texture* mTexture;					// テクスチャ

	unsigned int mVAO;					// VAO
	//float* mMatricesBuffer;
	//unsigned int mMaxActorNum;
	unsigned int mInstanceVAO;
	unsigned int mMeshVAO;
	unsigned int mMeshIndicesNum;
};

