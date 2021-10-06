#pragma once
#include "PlayerBase.h"
#include <iostream>

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class PlayerBehaviorComponent;
class InstanceBase;
class PlayerBullet;
class LaserEffect;
class ObjectPool;

class Player1 : public PlayerBase
{
public:
	Player1();
	~Player1();

	virtual void UpdateActor(float deltaTime) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;

	// キャラ情報読み込み関連
	void LoadResource();
	void LoadModel()override;
	void LoadSkeleton()override;
	void LoadAnimation()override;

	void BehaviorResister() override;

	void SetCollider()override;

private:
	// エイム用ターゲット
	GameObject* mTarget;

	PlayerState mNowState;							// 現在のステート
	PlayerState mNextState;							// 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール
	LaserEffect* mLaser;

	Vector3 mVelocityVec;

	PlayerBullet* mBullet;

	static const float m_range;
	bool mAimMode;

	float mSpecialShotTimer;
	static const float mSpecialShotInterval;

	
};