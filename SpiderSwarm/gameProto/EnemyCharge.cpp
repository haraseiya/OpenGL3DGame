#include "EnemyCharge.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

const float EnemyCharge::mShotInterval = 0.5f;

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

	// 5秒おきにプレイヤーに向かって発射
	mShotTimer += deltaTime;
	const bool isShot = mShotTimer > mShotInterval;
	if (isShot)
	{
		mShotTimer = 0.0f;

		// 弾生成
		mBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
	}

	// 自身の位置と次に向かう位置を取得
	Vector3 enemyPos = mOwner->GetPosition();

	// 次に向かう位置に自分を移動させる
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, 0.01f));

	return EnemyStateEnum::Attack2;
}

void EnemyCharge::OnEnter()
{
	// 死亡アニメーション再生
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
}

void EnemyCharge::OnExit()
{
}
