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
	// �C���X�^���X�̃^�C�v
	enum class InstanceType
	{
		PlayerBullet = 0,	// �v���C���[�̒e
		EnergyCapsule,		// �G�l���M�[�J�v�Z��

		InstanceTypeNum
	};

	// 
	InstanceMeshManager(Mesh* mesh, unsigned int maxInstanceNum);
	~InstanceMeshManager();
	
	// �C���X�^���X���b�V�����Z�b�g
	void SetInstanceMesh();

	// �s��o�b�t�@������
	void PreparationVAO();

	// �V�F�[�_�[�̃Z�b�g
	void SetShader();

private:
	// �C���X�^���X�Q
	struct Instance
	{
		std::vector<InstanceMeshComponent*> mInstanceMeshComp;
		Shader* mInstanceShader;			// �V�F�[�_�[
		Mesh* mMesh;						// ���b�V��
		Texture* mTexture;					// �e�N�X�`��

		unsigned int mVAO;					// VAO
		float* mMatricesBuffer;				
		unsigned int mMaxActorNum;
		unsigned int mInstanceVAO;
		unsigned int mMeshVAO;
		unsigned int mMeshIndicesNum;
	};

	Instance* mInstance;

	unsigned int mInstanceVAO;					// �C���X�^���XVAO
	const int mMaxInstance;							// �ő�C���X�^���X��
	static const unsigned int mMarixElemNum;
	static const size_t mMatrix4Size;			// �s��̑傫��

	const int mMatRowNum;
	const int mMatColorNum;
	const int mStartAttrib;
	const int mInstanceTypeNum;

	//std::vector<GameObject*> mGameObject;
	std::unordered_map<InstanceType, Instance> mInstanceMap;
	Texture* mTexture;
};

