#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "InstanceBase.h"

const unsigned int InstanceMeshManager::mMarixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(InstanceBase* instance,unsigned int maxInstance)
	: mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceBase::InstanceType::InstanceTypeNum))
	, mMaxInstance(maxInstance)
{
	// �C���X�^���X�V�F�[�_�[�ǂݍ���
	mInstanceShader = new Shader();
	mInstanceShader->Load("Shaders/InstanceMesh.vert", "Shaders/InstanceMesh.frag");
	mInstances.push_back(instance);

	// ���_�z��̏���
	PreparationVAO();
	SetInstanceMesh();
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh()
{
	for (int i = 0; i < mInstances.size(); i++)
	{
		mVAO = mInstances[i]->GetMesh()->GetVertexArray()->GetVAO();
		mIndexNum = mInstances[i]->GetMesh()->GetVertexArray()->GetNumIndices();

		glGenBuffers(1, &mInstanceVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);

		glBufferData(GL_ARRAY_BUFFER, mMaxInstance * mMatrix4Size, mBufferMatrices, GL_DYNAMIC_DRAW);
		glBindVertexArray(mVAO);

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
}

// VAO�̏���
void InstanceMeshManager::PreparationVAO()
{
	// ���b�V�����Ȃ����
	//if (!mInstances.empty()) return;
	mBufferMatrices = new float[mMaxInstance * mMarixElemNum];

	// �C���X�^���X��ނ̑���
	for (int i = 0; i < mMaxInstance; i++)
	{
		Matrix4 mat = mInstances[i]->GetWorldTransform();

		// �s��̍s�Ɨ��]�u����
		mat.Transpose();
		memcpy(&(mInstances[mMarixElemNum]), mat.GetAsFloatPtr(), mMatrix4Size);
	}

	// �s��o�b�t�@�ɃR�s�[
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mMaxInstance * mMatrix4Size, &mBufferMatrices[0]);
}

void InstanceMeshManager::SetShader()
{
	// �r���[�A�v���W�F�N�V�����s��
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view*proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// mInstances.size()�������Ă邩�s��
	for (int i = 0; i < mMaxInstance; i++)
	{
		// �e�N�X�`���̃Z�b�g
		mTexture = mInstances[i]->GetMesh()->GetTexture(0);
		if (mTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// �C���X�^���V���O�`��
		glBindVertexArray(mVAO);
		glDrawElementsInstanced(GL_TRIANGLES, mIndexNum, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mMaxInstance));
		glBindVertexArray(0);
	}
}
