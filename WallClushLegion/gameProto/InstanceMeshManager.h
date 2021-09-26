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
	
	// �C���X�^���X���b�V�����Z�b�g
	void SetInstanceMesh();

	// �s��o�b�t�@������
	void PreparationVAO();

	// �V�F�[�_�[�̃Z�b�g
	void SetShader();

private:
	// �C���X�^���X�Q
	//struct Instance
	//{
	//	std::vector<InstanceMeshComponent*> mInstanceMeshComp;
	//	Shader* mInstanceShader;			// �V�F�[�_�[
	//	Mesh* mMesh;						// ���b�V��
	//	Texture* mTexture;					// �e�N�X�`��

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
	unsigned int mInstanceVAO;					// �C���X�^���XVAO
	unsigned int mMaxInstance;							// �ő�C���X�^���X��
	static const unsigned int mMarixElemNum;
	static const size_t mMatrix4Size;			// �s��̑傫��

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

