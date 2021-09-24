#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "InstanceBase.h"

const unsigned int InstanceMeshManager::mMarixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(std::vector<InstanceBase*> instances,unsigned int maxInstanceNum)
	: mMaxInstance(maxInstanceNum)
	, mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
{
	// �C���X�^���X�V�F�[�_�[�ǂݍ���
	mInstanceShader = new Shader();
	mInstanceShader->Load("Shaders/InstanceMesh.vert", "Shaders/InstanceMesh.frag");

	// �s��o�b�t�@�̍쐬
	mBufferMatrices = new Matrix4[mMaxInstance * mMarixElemNum];

	for (int i = 0; i < instances.size(); i++)
	{
		Matrix4 model=Matrix4::Identity;
		mInstances = instances;
		model = mInstances[i]->GetWorldTransform();
		mBufferMatrices[i] = model;
	}

	// �C���X�^���X�̒��_�z��𐶐�
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMaxInstance * mMatrix4Size, &mBufferMatrices[0], GL_STATIC_DRAW);

	for (int i = 0; i < mInstances.size(); i++)
	{
		// VAO�̎擾
		glBindVertexArray(mInstances[i]->GetMesh()->GetVertexArray()->GetVAO());

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

	//// �Z�b�g
	//SetInstanceMesh();
	//PreparationVAO();
	//SetShader();
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh()
{
}

//// VAO�̏���
//void InstanceMeshManager::PreparationVAO()
//{
//	// �C���X�^���X��ނ̑���
//	const int instanceTypeNum = static_cast<int>(InstanceType::InstanceTypeNum);
//	for (int i=0;i<instanceTypeNum;i++)
//	{
//		int num = 0;
//		for (auto g : mInstance)
//		{
//			Matrix4 mat = g->GetWorldTransform();
//			
//			// �s��̍s�Ɨ��]�u����
//			mat.Transpose();
//			memcpy(&(mInstance[num*mMarixElemNum]),mat.GetAsFloatPtr(),mMatrix4Size);
//
//
//			// �I�u�W�F�N�g�i���o�[
//			++num;
//		}
//	}
//
//	// �C���X�^���X��ނ��Ƃɍs��o�b�t�@���R�s�[
//	for (int i = 0; i < instanceTypeNum; i++)
//	{
//		// �s��o�b�t�@�ɃR�s�[
//		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, mMaxInstance * mMatrix4Size, &mModel[0]);
//	}
//}

void InstanceMeshManager::SetShader()
{
	// �r���[�A�v���W�F�N�V�����s��
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view*proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// �C���X�^���X�`��
	for (int i = 0; i < mInstances.size(); i++)
	{
		// �e�N�X�`���̃Z�b�g
		mTexture = mInstances[i]->GetMesh()->GetTexture(0);
		if (mTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// �C���X�^���V���O�`��
		glBindVertexArray(mInstances[i]->GetMesh()->GetVertexArray()->GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, mInstances[i]->GetMesh()->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mMaxInstance));
		glBindVertexArray(0);
	}
}
