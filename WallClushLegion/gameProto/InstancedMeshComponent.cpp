#include "InstancedMeshComponent.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// �C���X�^���X�I�u�W�F�N�g�̑���
const int InstancedMeshComponent::mAmount = 100;

InstancedMeshComponent::InstancedMeshComponent(GameObject* owner,Tag tag, const int& amount)
	: Component(owner)
	, mOwner(owner)
{
	// �����_�[�N���X�ɃC���X�^���X���b�V���R���|�[�l���g��ǉ�
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);

	// �Q�[���I�u�W�F�N�g�������̃������m��
	mModelMatrices.reserve(mAmount);

	//// �C���X�^���X�z�񐶐�
	//glGenBuffers(1,&mBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	//glBufferData(GL_ARRAY_BUFFER, mAmount * sizeof(GameObject), &mModelMatrices[0], GL_STATIC_DRAW);

	//// ���_�z����A�N�e�B�u��
	//mVertexArray = mMesh->GetVertexArray();
	//mVertexArray->SetActive();
}

InstancedMeshComponent::~InstancedMeshComponent()
{
	// �C���X�^���X���b�V���R���|�[�l���g��j��
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
}

void InstancedMeshComponent::Update(float deltaTime)
{
	// ���_�z����擾
	mVertexArray = mMesh->GetVertexArray();
	mVertexArray->SetActive();

	// ���_�����z�����
	for (unsigned int i = 0; i < mVertexArray->GetNumIndices(); i++)
	{
		glDrawElementsInstanced(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, 0, mAmount);
	}
}

void InstancedMeshComponent::Draw(Shader* shader)
{
	// ���b�V�������݂��Ȃ���ΕԂ�
	if (!mMesh) return;

	// ���[���h�ϊ����Z�b�g
	shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

	// �X�y�L�������x�Z�b�g
	shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

	// ���~�i���X���x�Z�b�g
	shader->SetFloatUniform("uLuminance", mMesh->GetLuminace());
}
