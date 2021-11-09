#pragma once
#include "GameObject.h"

class Mesh;
class MeshComponent;

class EnemySpawnObject : public GameObject
{
public:
	EnemySpawnObject(const Vector3& pos);
	~EnemySpawnObject();

	const Vector3& GetPosition() const { return mPosition; }

private:
	Mesh* mMesh;
	MeshComponent* mMeshComp;
};

