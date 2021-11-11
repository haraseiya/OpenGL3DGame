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
	EnemyBullet* mEnemyBullet;
	Vector3 mTargetPos;
	std::vector<Vector3> mTargetPositions;

	int mTargetPosNum;

	static const float mShotInterval;
	static const float mChargeSpeed;	// �ːi�X�s�[�h
	float mShootTimer;
};

