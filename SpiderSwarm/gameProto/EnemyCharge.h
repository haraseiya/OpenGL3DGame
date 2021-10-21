#pragma once

#include "EnemyState.h"

class EnemyBehaviorComponent;
class EnemyBullet;

class EnemyCharge :public EnemyState
{
public:
	EnemyCharge(EnemyBehaviorComponent* owner);
	~EnemyCharge();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

private:
	EnemyBullet* mBullet;
	Vector3 mTargetPos;

	static const float mShotInterval;
	float mShotTimer;
};

