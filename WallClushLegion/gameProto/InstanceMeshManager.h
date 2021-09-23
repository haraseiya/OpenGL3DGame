#pragma once

#include <vector>

class Mesh;
class Shader;
class Texture;
class GameObject;
class InstanceMeshComponent;

class InstanceMeshManager
{
public:
	// インスタンスのタイプ
	enum class InstanceType
	{
		PlayerBullet = 0,	// プレイヤーの弾
		EnergyCapsule,		// エネルギーカプセル

		InstanceTypeNum
	};

	// 
	InstanceMeshManager(Mesh* mesh, unsigned int maxInstanceNum);
	~InstanceMeshManager();
	
	// インスタンスメッシュをセット
	void SetInstanceMesh();

	// 行列バッファを準備
	void PreparationVAO();

	// シェーダーのセット
	void SetShader();

private:
	// インスタンス群
	struct Instance
	{
		std::vector<InstanceMeshComponent*> mInstanceMeshComp;
		Shader* mInstanceShader;			// シェーダー
		Mesh* mMesh;						// メッシュ
		Texture* mTexture;					// テクスチャ

		unsigned int mVAO;					// VAO
		float* mMatricesBuffer;				
		unsigned int mMaxActorNum;
		unsigned int mInstanceVAO;
		unsigned int mMeshVAO;
		unsigned int mMeshIndicesNum;
	};

	Instance* mInstance;

	unsigned int mInstanceVAO;					// インスタンスVAO
	const int mMaxInstance;							// 最大インスタンス数
	static const unsigned int mMarixElemNum;
	static const size_t mMatrix4Size;			// 行列の大きさ

	const int mMatRowNum;
	const int mMatColorNum;
	const int mStartAttrib;
	const int mInstanceTypeNum;

	//std::vector<GameObject*> mGameObject;
	std::unordered_map<InstanceType, Instance> mInstanceMap;
	Texture* mTexture;
};

