#pragma once
#include "EffectBase.h"

class Mesh;
class SkeletalMeshComponent;

class EnemySpawnerEffect : public EffectBase
{
public:
	EnemySpawnerEffect(const Vector3& pos);
	~EnemySpawnerEffect();

	const Vector3& GetPosition() const { return mPosition; }

private:
	Mesh* mMesh;
	SkeletalMeshComponent* mMeshComp;
};

