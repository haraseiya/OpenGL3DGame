#pragma once

#include "GameObject.h"

// シーンごとのプレイヤー
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,
	PLAYER_GAMESCENE,
	PLAYER_RESULT,
};

// プレイヤーの状態
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,   // 待機
	PLAYER_STATE_RUN,   // 走る
	PLAYER_STATE_ATTACK,
	PLAYER_STATE_REVIVE,
	PLAYER_STATE_NUM,   // 総アニメーション数
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class ChantEffect;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	void UpdateActor(float deltaTime) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other) override;

protected:
	GameObject* mTarget;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;				// プレイヤーの攻撃当たり判定ボックス   
	EffectComponent* mEffect;
	ChantEffect* mChantEffect;

	PlayerState                              mNowState;        // 現在のステート
	PlayerState                              mNextState;       // 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール

	Vector3                                  mVelocityVec;

	static const float m_range;
	float mShootTimer;
	bool mAimMode;
	static const float mInterval;
};

