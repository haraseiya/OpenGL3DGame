#pragma once

class Mesh;
class Shader;

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

	InstanceMeshManager(Mesh* mesh, unsigned int maxInstanceNum);
	~InstanceMeshManager();
	
	// インスタンスメッシュをセット
	void SetInstanceMesh(Mesh* mesh,unsigned int maxInstanceNum);

	// 行列バッファを準備
	void PreparationVAO();

	// シェーダーのセット
	void SetShader();

private:
	static const size_t mMatrix4Size;	// 行列の大きさ
	const int mMaxInstance;				// 最大インスタンス数

	unsigned int mInstanceVAO;

	const int mMatRowNum;
	const int mMatColorNum;
	const int mStartAttrib;
	const int mInstanceTypeNum;

	Mesh* mMesh;
	Shader* mInstanceShader;
};

