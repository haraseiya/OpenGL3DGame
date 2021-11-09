#include "EnemySpawnObject.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"

EnemySpawnObject::EnemySpawnObject(const Vector3& pos)
{
	// �|�W�V�����̃Z�b�g
	mPosition = pos;
	mScale = 100.0f;

	//���b�V���̃��[�h
	mMesh = RENDERER->GetMesh("Assets/Item/Skeleton.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

EnemySpawnObject::~EnemySpawnObject()
{
}
