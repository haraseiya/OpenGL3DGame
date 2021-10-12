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
	RunForward, // 前走り
	RunRight,	// 右走り
	RunLeft,	// 左走り
	RunBackward,// 後ろ走り
	LookAround, // 見回す
	Attack1,    // 攻撃１
	Attack2,    // 攻撃２
	Attack3,    // 攻撃３
	GetDamage,  // ダメージ受けた
	Roar,       // 吠える
	Stun,       // 気絶
	Die,        // 死亡
	Hold,		// 銃を構える
	StateNum	// 状態の総数
};

// シーンごとのプレイヤー
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,	// タイトルシーン
	PLAYER_SELECTSCENE,		// セレクトシーン
	PLAYER_GAMESCENE,		// ゲームシーン
	PLAYER_RESULT,			// リザルトシーン
};

// プレイヤーの状態
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,		// 待機
	PLAYER_STATE_RUN_FORWARD,	// 走る
	PLAYER_STATE_RUN_RIGHT,		// 右走り
	PLAYER_STATE_RUN_LEFT,		// 左走り
	PLAYER_STATE_RUN_BACKWARD,	// 後ろ走り
	PLAYER_STATE_DIE,			// 死亡
	PLAYER_STATE_VICTORY,		// 勝利ポーズ
	PLAYER_STATE_NUM,			// 総アニメーション数
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;
class SceneBase;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;

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

	// HPのゲッターセッター
	int GetHitPoint() { return mHitPoint; }
	void SetHitPoint(int hp) { mHitPoint = hp; }

	bool GetDeadAnimFlag() { return mIsDeadAnimFinish; }
	void SetDeadAnimFlag(bool isDeadAnimFinish) { mIsDeadAnimFinish = isDeadAnimFinish; }

protected:
	GameObject* mTarget;
	AABB mPlayerBox;
	Mesh* mMesh;
	SceneBase* mSceneBase;

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

	PlayerBehaviorComponent* mPlayerBehavior;	// プレイヤーのふるまい

	Vector3 mVelocityVec;

	static const float mRange;
	static const float mAnimationSpeed;
	static const float mInterval;
	static const float mSpecialShotInterval;

	float mShootTimer;
	float mSpecialShotTimer;

	bool mAimMode;

	int mHitPoint;
	bool mIsDeadAnimFinish;
};

