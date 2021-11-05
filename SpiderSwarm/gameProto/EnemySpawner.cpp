#include "EnemySpawner.h"
#include "EnemyBase.h"

EnemySpawner::EnemySpawner(EnemyBase* proto)
	:mProtoType(proto)
{
}

EnemySpawner::~EnemySpawner()
{
}

EnemyBase* EnemySpawner::SpawnEnemy(const Vector3& spawnPos)
{
	//mSpawnPosition = spawnPos;
	mProtoType->SetPosition(spawnPos);
	return mProtoType->Clone();
}
