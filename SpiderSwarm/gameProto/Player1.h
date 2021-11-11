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

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	// キャラ情報読み込み関連
	void LoadModel() override;
	void LoadSkeleton() override;
	void LoadAnimation() override;

	void BehaviorResister() override;

	void SetCollider()override;

private:

	void AttachWeapon();	// 武器をアタッチ

	AttachMeshComponent* mWeaponMesh;
	Mesh* mWeapon;

	PlayerState mNowState;							// 現在のステート
	PlayerState mNextState;							// 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール
	LaserEffect* mLaser;

	Vector3 mVelocityVec;

	PlayerBullet* mBullet;

	static const float m_range;
	bool mAimMode;
};