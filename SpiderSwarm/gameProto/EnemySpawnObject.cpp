#include "EnemySpawnObject.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"

EnemySpawnObject::EnemySpawnObject(const Vector3& pos)
{
	// ポジションのセット
	mPosition = pos;
	mScale = 100.0f;

	//メッシュのロード
	mMesh = RENDERER->GetMesh("Assets/Item/Skeleton.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

EnemySpawnObject::~EnemySpawnObject()
{
}
