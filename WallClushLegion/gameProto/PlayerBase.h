#pragma once

#include "GameObject.h"
#include "BoxCollider.h"

// プレイヤークラス状態enum 状態に変更があったらstatePlayerNameも変更
enum class PlayerStateEnum : unsigned char
{
	Invalid,    // 無効なステート
	Spawn,      // 出現
	Idle,       // 待機状態
	Walk,       // 歩く
	Turn,       // ターン
	Run,        // 走る
	LookAround, // 見回す
	Attack1,    // 攻撃１
	Attack2,    // 攻撃２
	Attack3,    // 攻撃３
	GetDamage,  // ダメージ受けた
	Roar,       // 吠える
	Stun,       // 気絶
	Die,        // 死亡
	StateNum	// 状態の総数
};

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
	PLAYER_STATE_NUM,   // 総アニメーション数
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other) override;

	// キャラ情報読み込み関連
	virtual void LoadModel() = 0;
	virtual void LoadSkeleton() = 0;
	virtual void LoadAnimation() = 0;

	// ふるまいを追加
	virtual void BehaviorResister() = 0;

	// 当たり判定を追加
	virtual void SetCollider() = 0;

	// ステート名を取得するヘルパー関数
	const char* GetPlayerStateEnumName(PlayerStateEnum state);

protected:
	GameObject* mTarget;
	AABB mPlayerBox;
	Mesh* mMesh;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;			
	EffectComponent* mEffect;

	PlayerState mNowState;        // 現在のステート
	PlayerState mNextState;       // 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール

	PlayerBehaviorComponent* mPlayerBehavior;

	Vector3 mVelocityVec;

	static const float mRange;
	static const float mAnimationSpeed;
	float mShootTimer;
	bool mAimMode;
};

