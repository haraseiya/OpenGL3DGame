#include "InstanceMeshManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;

InstanceMeshManager::InstanceMeshManager(Mesh* mesh,unsigned int maxInstanceNum)
	: mMaxInstance(maxInstanceNum)
	, mMatRowNum(4)
	, mMatColorNum(4)
	, mStartAttrib(3)
	, mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
	, mMesh(mesh)
{
}

InstanceMeshManager::~InstanceMeshManager()
{

}

void InstanceMeshManager::SetInstanceMesh(Mesh* mesh, unsigned int maxInstanceNum)
{
	// �C���X�^���X�̒��_�z��𐶐�
	glGenBuffers(1, &mInstanceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVAO);

	// �s��o�b�t�@��InstanceVAO�ƃo�C���h
	glBufferData(GL_ARRAY_BUFFER, mMatrix4Size * mMaxInstance,/*�s��o�b�t�@�̐擪�f�[�^*/ , GL_STATIC_DRAW);
	glBindVertexArray(mesh->GetVertexArray()->GetNumVerts()/*���b�V��VAO*/);

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
	// �C���X�^���X�^�C�v�̑���
	const int instanceTypeNum = static_cast<int>(InstanceType::InstanceTypeNum);
	for (int i=0;i<instanceTypeNum;i++)
	{
		int num = 0;
		for (int i=0;i<)
		{

		}
	}
}

void InstanceMeshManager::SetShader()
{
	// �r���[�A�v���W�F�N�V�����s��
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 proj = RENDERER->GetProjectionMatrix();

	// �C���X�^���X�V�F�[�_�[
	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", view * proj);
	mInstanceShader->SetIntUniform("uTexture", 0);

	// �S��ނ̃C���X�^���X�`��
	for (int i = 0; i < mInstanceTypeNum; i++)
	{
		// �e�N�X�`���̃Z�b�g
		Texture* t = mMesh->GetTexture(0);
		if (t)
		{
			glActiveTexture(GL_TEXTURE0);
			continue;
		}

		// �C���X�^���V���O�`��
		glBindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES,,GL_UNSIGNED_INT,0,static_cast<GLsizei>());
		glBindVertexArray(0);
	}
}
