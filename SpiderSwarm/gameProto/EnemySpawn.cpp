#include "EnemySpawn.h"

EnemySpawn::EnemySpawn(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Spawn;
}

EnemySpawn::~EnemySpawn()
{
}

EnemyStateEnum EnemySpawn::Update(float deltaTime)
{
	// アニメーション終了時、走りアニメーションに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		// ボス敵であれば咆哮、それ以外なら待機状態に飛ばす
		if (mOwnerActor->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
		{
			return EnemyStateEnum::Roar;
		}
		else
		{
			return EnemyStateEnum::Idle;
		}
	}

	return EnemyStateEnum::Spawn;
}

void EnemySpawn::OnEnter()
{
	// 出現アニメーション再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Spawn);
}

void EnemySpawn::OnExit()
{

}

