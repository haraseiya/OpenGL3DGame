#pragma once
#include "Math.h"

class EnemyBase;

class EnemySpawner
{
public:
	EnemySpawner(EnemyBase* proto);
	~EnemySpawner();

	EnemyBase* SpawnEnemy(const Vector3& spawnPos);

private:
	EnemyBase* mProtoType;	// 敵のプロトタイプ
	Vector3 mSpawnPosition;
};

