#include "EnemyDeath.h"
#include "ExplosionEffect.h"
#include "Score.h"
#include "ExperienceItem.h"

EnemyDeath::EnemyDeath(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
	, mExplosion(nullptr)
	, mExperienceItem(nullptr)
{
	mStateType = EnemyStateEnum::Death;
}

EnemyDeath::~EnemyDeath()
{
	delete mExplosion;
}

EnemyStateEnum EnemyDeath::Update(float deltaTime)
{
	// 死亡アニメーションが終了したら
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Invalid;
	}

	return EnemyStateEnum::Death;
}

void EnemyDeath::OnEnter()
{
	// 死亡アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Death);
	//mOwner->RemoveCollider();
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK) 
	//{ 
	//	mScore = 1000; 
	//}
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG) 
	//{	
	//	mScore = 5000;
	//}
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	//{
	//	mScore = 20000;
	//}
	//mSumScore += mScore;
}

// 死亡アニメーション終了時の処理
void EnemyDeath::OnExit()
{
	// 自身を破棄
	mOwner->SetState(GameObject::STATE_PAUSED);
	mOwner->SetPosition(Vector3(1000, 0, 500));
	mExplosion = new ExplosionEffect(mOwner->GetPosition());
	mExperienceItem = new ExperienceItem(mOwner->GetPosition());
}
