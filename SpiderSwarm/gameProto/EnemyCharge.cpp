#include "EnemyCharge.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

const float EnemyCharge::mShotInterval = 1.5f;

EnemyCharge::EnemyCharge(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack2;
	mTargetPos[0] = Vector3(2000,2000,750);
	mTargetPos[1] = Vector3(-2000, 2000, 750);
	mTargetPos[2] = Vector3(-2000, -2000, 750);
	mTargetPos[3] = Vector3(2000, -2000, 750);

	mRandomNumber = Math::GetRandom(0, 3);
}

EnemyCharge::~EnemyCharge()
{
}

EnemyStateEnum EnemyCharge::Update(float deltaTime)
{
	// 体力が0以下なら死亡状態へ遷移
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// アニメーションが終了したら
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	// １秒おきに弾幕発射
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// 弾生成
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
		}
	}

	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		// 5秒おきにプレイヤーに向かって弾を発射
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;		// 右上
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// 左上
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// 右下
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// 左下
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// 敵弾のインスタンス生成
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

	// 自身の位置と次に向かう位置を取得
	Vector3 enemyPos = mOwner->GetPosition();
	enemyPos.z = 750.0f;
	Vector3 direction = mTargetPos[mRandomNumber] - enemyPos;
	direction.Normalize();
	Vector3 lerpDirection = Vector3::Lerp(mOwner->GetForward(), direction, 1.0f);

	// 次に向かう位置に自分を移動させる
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos[mRandomNumber], 0.01f));
	mOwner->RotateToNewForward(lerpDirection);

	// 攻撃状態を返す
	return EnemyStateEnum::Attack2;
}

void EnemyCharge::OnEnter()
{
	// 死亡アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
	mTargetPos[mRandomNumber] = mOwner->GetPosition() + Vector3(Math::GetRandom(-1000.0f, 1000.0f), Math::GetRandom(-1000.0f, 1000.0f), 0);
}

void EnemyCharge::OnExit()
{
}

