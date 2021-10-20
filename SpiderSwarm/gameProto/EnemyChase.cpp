#include "EnemyChase.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "GameScene.h"

const float EnemyChase::mRange = 10000.0f;

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
{
	// 敵のアニメーション状態を走りに設定
	mStateType = EnemyStateEnum::Run;
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	// 体力が0以下の場合死亡アニメーションへ遷移
	if (mOwnerActor->GetHitPoint() <= 0)
	{
		return EnemyStateEnum::Death;
	}

	// ターゲットが存在していなければIdle状態に移行
	const bool isIdle = !mTarget /*|| Math::Abs(mTarget->GetPosition().LengthSq() - mOwnerActor->GetPosition().LengthSq()) > mRange*/;
	if (!mTarget)
	{
		return EnemyStateEnum::Idle;
	}

	// キャラクターの前方を取得
	Vector3 charaForwardVec = mOwnerActor->GetForward();

	// プレイヤーへの向きを求める
	Vector3 enemyPos = mOwnerActor->GetPosition();
	enemyPos.z = 500;
	Vector3 playerPos = mTarget->GetPosition();
	Vector3 direction = playerPos- enemyPos;
	direction.Normalize();

	// ある程度離れている場合
	if (direction.LengthSq() > 0.5f)
	{
		// プレイヤーの方向へ向かう
		enemyPos += direction * 100.0f * deltaTime;
		mOwnerActor->SetPosition(enemyPos);

		// 方向キー入力
		charaForwardVec = Vector3::Lerp(mOwnerActor->GetForward(), direction, 0.1f);
		// 進行方向に向けて回転
		charaForwardVec.Normalize();

		mOwnerActor->RotateToNewForward(charaForwardVec);
	}

	// 続行
	return EnemyStateEnum::Run;
}

void EnemyChase::OnEnter()
{
	// 走りアニメーション再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}