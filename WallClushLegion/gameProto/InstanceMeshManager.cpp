#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

const unsigned int InstanceMeshManager::mMarixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(Mesh* mesh,unsigned int maxInstanceNum)
	: mMaxInstance(maxInstanceNum)
	, mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
{
	// �Z�b�g
	SetInstanceMesh();
	PreparationVAO();
	SetShader();
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh()
{
	// VAO�̎擾
	mInstance->mVAO = mInstance->mMesh->GetVertexArray()->GetVAO();

	// �C���f�b�N�X���̎擾
	mInstance->mIndexNum = mInstance->mMesh->GetVertexArray()->GetNumIndices();

	// �s��o�b�t�@�̍쐬
	mInstance = new Instance[mMaxInstance * mMarixElemNum];

	// �C���X�^���X�̒��_�z��𐶐�
	glGenBuffers(1, &mInstanceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);

	// �s��o�b�t�@��InstanceVAO�ƃo�C���h
	glBufferData(GL_ARRAY_BUFFER, mMaxInstance * mMatrix4Size, &mInstance[0], GL_STATIC_DRAW);
	glBindVertexArray(mInstance->mVAO);

	// ���_�A�g���r���[�g�ɍs����Z�b�g
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(1 * sizeof(float) * 4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(2 * sizeof(float) * 4));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(3 * sizeof(float) * 4));

	// �����P�F���_�z��̃C���f�b�N�X���w��
	// �����Q�F�X���b�g�ł̔ėp�����̍X�V���œn�����C���X�^���X�̐����w�肷��
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
}

// VAO�̏���
void InstanceMeshManager::PreparationVAO()
{
	// �C���X�^���X��ނ̑���
	const int instanceTypeNum = static_cast<int>(InstanceType::InstanceTypeNum);
	for (int i=0;i<instanceTypeNum;i++)
	{
		int num = 0;
		for (auto g : mInstance)
		{
			Matrix4 mat = g->GetWorldTransform();
			
			// �s��̍s�Ɨ��]�u����
			mat.Transpose();
			memcpy(&(mInstance[num*mMarixElemNum]),mat.GetAsFloatPtr(),mMatrix4Size);


			// �I�u�W�F�N�g�i���o�[
			++num;
		}
	}

	// �C���X�^���X��ނ��Ƃɍs��o�b�t�@���R�s�[
	for (int i = 0; i < instanceTypeNum; i++)
	{
		// �s��o�b�t�@�ɃR�s�[
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mMaxInstance * mMatrix4Size, &mModel[0]);
	}
}

void InstanceMeshManager::SetShader()
{
	// �r���[�A�v���W�F�N�V�����s��
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	// �C���X�^���X�V�F�[�_�[
	mInstanceShader->Load("InstanceMesh.vert","InstanceMesh.frag");
	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view * proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// �S��ނ̃C���X�^���X�`��
	for (int i = 0; i < mInstanceTypeNum; i++)
	{
		// �e�N�X�`���̃Z�b�g
		mTexture = mMesh->GetTexture(0);
		if (mTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// �C���X�^���V���O�`��
		glBindVertexArray(mVAO);
		glDrawElementsInstanced(GL_TRIANGLES, mMesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mMaxInstance));
		glBindVertexArray(0);
	}
}
