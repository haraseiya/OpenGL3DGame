#include "EnemyDeath.h"
#include "ExplosionEffect.h"
#include "ScoreManager.h"
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
	// ���S�A�j���[�V�������I��������
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Spawn;
	}

	return EnemyStateEnum::Death;
}


void EnemyDeath::OnEnter()
{
	// ���S�A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Death);


	//mOwner->RemoveCollider();
}

// ���S�A�j���[�V�����I�����̏���
void EnemyDeath::OnExit()
{
	int score = 0;
	// ���g�𖢎g�p��Ԃֈڍs
	mOwner->SetIsActive(false);

	// �����G�t�F�N�g����
	mExplosion = new ExplosionEffect();
	mExplosion->LoadEffect();
	mExplosion->CreateEffect();
	mExplosion->SetPosition(mOwner->GetPosition());
	mExplosion->SetRelativePos();

	mExperienceItem = new ExperienceItem(mOwner->GetPosition());
	mOwner->SetPosition(Vector3(0,0,5000));

	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		ScoreManager::GetInstance()->AddScore(1000);
		mSumScore += 1000;
	}
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		ScoreManager::GetInstance()->AddScore(5000);
		mSumScore += 5000;
	}
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		ScoreManager::GetInstance()->AddScore(20000);
		mSumScore += 20000;
		mOwner->SetIsBossDeadFlag(true);
	}

	mOwner->SetState(GameObject::STATE_PAUSED);
}
