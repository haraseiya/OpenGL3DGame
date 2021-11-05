#include "EnemyCharge.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

const float EnemyCharge::mShotInterval = 1.5f;

EnemyCharge::EnemyCharge(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack2;
	mTargetPos = mOwner->GetPosition() + Vector3(Math::GetRandom(-1000.0f, 1000.0f), Math::GetRandom(-1000.0f, 1000.0f), 500.0f);
}

EnemyCharge::~EnemyCharge()
{
}

EnemyStateEnum EnemyCharge::Update(float deltaTime)
{
	// ‘Ì—Í‚ª0ˆÈ‰º‚È‚çŽ€–Só‘Ô‚Ö‘JˆÚ
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ªI—¹‚µ‚½‚ç
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	// ‚P•b‚¨‚«‚É’e–‹”­ŽË
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// ’e¶¬
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
		}
	}

	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		// 5•b‚¨‚«‚ÉƒvƒŒƒCƒ„[‚ÉŒü‚©‚Á‚Ä’e‚ð”­ŽË
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;			// ‰Eã
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// ¶ã
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// ‰E‰º
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// ¶‰º
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// “G’e‚ÌƒCƒ“ƒXƒ^ƒ“ƒX¶¬
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerLeft), 2.0f, 300.0f);
		}
	}

	// Ž©g‚ÌˆÊ’u‚ÆŽŸ‚ÉŒü‚©‚¤ˆÊ’u‚ðŽæ“¾
	Vector3 enemyPos = mOwner->GetPosition();
	Vector3 direction = mTargetPos - enemyPos;
	direction.Normalize();
	Vector3 lerpDirection = Vector3::Lerp(mOwner->GetForward(), direction, 1.0f);

	// ŽŸ‚ÉŒü‚©‚¤ˆÊ’u‚ÉŽ©•ª‚ðˆÚ“®‚³‚¹‚é
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, 0.01f));
	mOwner->RotateToNewForward(lerpDirection);

	// UŒ‚ó‘Ô‚ð•Ô‚·
	return EnemyStateEnum::Attack2;
}

void EnemyCharge::OnEnter()
{
	// Ž€–SƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
	mTargetPos = mOwner->GetPosition() + Vector3(Math::GetRandom(-1000.0f, 1000.0f), Math::GetRandom(-1000.0f, 1000.0f), 0);
}

void EnemyCharge::OnExit()
{
}

