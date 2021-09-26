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
	// �C���X�^���X�̃^�C�v
	enum class InstanceType
	{
		PlayerBullet = 0,	// �v���C���[�̒e
		EnergyCapsule,		// �G�l���M�[�J�v�Z��

		InstanceTypeNum
	};

	InstanceBase(Tag tag, Mesh* mesh);
	~InstanceBase();

	Mesh* GetMesh() { return mMesh; }

protected:
	InstanceMeshComponent* mInstanceMeshComp;
	InstanceMeshManager* mInstanceMeshManager;
	Shader* mInstanceShader;			// �V�F�[�_�[
	Mesh* mMesh;						// ���b�V��
	MeshComponent* mMeshComp;			// ���b�V���R���|�[�l���g
	Texture* mTexture;					// �e�N�X�`��

	unsigned int mVAO;					// VAO
	//float* mMatricesBuffer;
	//unsigned int mMaxActorNum;
	unsigned int mInstanceVAO;
	unsigned int mMeshVAO;
	unsigned int mMeshIndicesNum;
};

