#pragma once

class Mesh;
class Shader;

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

	InstanceMeshManager(Mesh* mesh, unsigned int maxInstanceNum);
	~InstanceMeshManager();
	
	// �C���X�^���X���b�V�����Z�b�g
	void SetInstanceMesh(Mesh* mesh,unsigned int maxInstanceNum);

	// �s��o�b�t�@������
	void PreparationVAO();

	// �V�F�[�_�[�̃Z�b�g
	void SetShader();

private:
	static const size_t mMatrix4Size;	// �s��̑傫��
	const int mMaxInstance;				// �ő�C���X�^���X��

	unsigned int mInstanceVAO;

	const int mMatRowNum;
	const int mMatColorNum;
	const int mStartAttrib;
	const int mInstanceTypeNum;

	Mesh* mMesh;
	Shader* mInstanceShader;
};

