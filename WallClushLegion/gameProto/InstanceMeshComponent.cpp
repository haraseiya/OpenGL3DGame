#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// インスタンスオブジェクトの総数
const int InstanceMeshComponent::mAmount = 100;

InstanceMeshComponent::InstanceMeshComponent(GameObject* owner,Mesh* mesh,const int& amount)
	: Component(owner)
	, mOwner(owner)
{
	// レンダークラスにインスタンスメッシュコンポーネントを追加
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);
	//mInstanceManager = new InstanceMeshManager(mesh,amount);
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	// インスタンスメッシュコンポーネントを破棄
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
}

void InstanceMeshComponent::Update(float deltaTime)
{
}

