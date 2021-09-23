#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// �C���X�^���X�I�u�W�F�N�g�̑���
const int InstanceMeshComponent::mAmount = 100;

InstanceMeshComponent::InstanceMeshComponent(GameObject* owner,Mesh* mesh,const int& amount)
	: Component(owner)
	, mOwner(owner)
{
	// �����_�[�N���X�ɃC���X�^���X���b�V���R���|�[�l���g��ǉ�
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);
	//mInstanceManager = new InstanceMeshManager(mesh,amount);
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	// �C���X�^���X���b�V���R���|�[�l���g��j��
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
}

void InstanceMeshComponent::Update(float deltaTime)
{
}

