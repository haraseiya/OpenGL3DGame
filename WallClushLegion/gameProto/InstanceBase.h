#pragma once
#include "GameObject.h"

class Mesh;
class Shader;
class Texture;
class MeshComponent;
class InstanceMeshComponent;

class InstanceBase : public GameObject
{
public:
	InstanceBase(Tag tag);
	~InstanceBase();

	Mesh* GetMesh() { return mMesh; }

protected:
	InstanceMeshComponent* mInstanceMeshComp;
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

