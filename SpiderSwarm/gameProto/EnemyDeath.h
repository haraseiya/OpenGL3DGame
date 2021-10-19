#pragma once
#include "EnemyState.h"

class ExplosionEffect;

class EnemyDeath:public EnemyState
{
public:
	EnemyDeath(EnemyBehaviorComponent* owner);
	~EnemyDeath();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

private:
	ExplosionEffect* mExplosion;
};

