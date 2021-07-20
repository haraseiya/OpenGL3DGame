#include "EnemyChase.h"
#include "Player.h"
#include "NPCActorBase.h"
#include "GameScene.h"

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
{
	mStateType = EnemyStateEnum::Run;
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	// ターゲットが存在していなければIdle状態に移行
	if (!mTarget)
	{
		return EnemyStateEnum::Idle;
	}

	// キャラクターの前方を取得
	Vector3 charaForwardVec = mOwnerActor->GetForward();

	// プレイヤーへの向きを求める
	Vector3 enemyPos = mOwnerActor->GetPosition();
	Vector3 npcPos = mTarget->GetPosition();
	Vector3 direction = npcPos- enemyPos;
	direction.Normalize();

	// ある程度離れている場合
	if (direction.LengthSq() > 0.5f)
	{
		// プレイヤーの方向へ向かう
		enemyPos += direction * 100.0f * deltaTime;
		mOwnerActor->SetPosition(enemyPos);

		// 方向キー入力
		charaForwardVec = direction;

		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		mOwnerActor->RotateToNewForward(charaForwardVec);
	}

	// 続行
	return EnemyStateEnum::Run;
}

void EnemyChase::OnEnter()
{
	// 見渡しアニメ再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}