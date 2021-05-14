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

	PLAYER_STATE_NUM,   // 総アニメーション数
};

class Player : public GameObject
{
public:
	Player();
	~Player();

	void UpdateActor(float deltaTime) override;
	void                         FixCollision(class BoxCollider* hitPlayerBox, class BoxCollider* hitBox);
	class SkeletalMeshComponent* GetSkeletalMeshComp();
	const class Animation* GetAnim(PlayerState state);

private:
	class SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider* mHitBox;
	class BoxCollider* mHitTrigger;
	class BoxCollider* mHitGroundBox;
	class BoxCollider* mHitHeadBox;
	class BoxCollider* mAttackBox;				// プレイヤーの攻撃当たり判定ボックス   
	class EffectComponent* mEffect;		

	PlayerState                              mNowState;        // 現在のステート
	PlayerState                              mNextState;       // 次のステート
	std::vector<class PlayerStateBase*> mStatePools;      // ステートクラスプール

	Vector3                                  mVelocityVec;

	static const float m_range;
};