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
	// �̗͂�0�ȉ��Ȃ玀�S��Ԃ֑J��
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// �A�j���[�V�������I��������
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	// �P�b�����ɒe������
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// �e����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
		}
	}

	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		// 5�b�����Ƀv���C���[�Ɍ������Ēe�𔭎�
		mShootTimer += deltaTime;
		const bool isShot = mShootTimer > mShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;			// �E��
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// ����
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// �E��
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// ����
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// �G�e�̃C���X�^���X����
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

	// ���g�̈ʒu�Ǝ��Ɍ������ʒu���擾
	Vector3 enemyPos = mOwner->GetPosition();
	Vector3 direction = mTargetPos - enemyPos;
	direction.Normalize();
	Vector3 lerpDirection = Vector3::Lerp(mOwner->GetForward(), direction, 1.0f);

	// ���Ɍ������ʒu�Ɏ������ړ�������
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, 0.01f));
	mOwner->RotateToNewForward(lerpDirection);

	// �U����Ԃ�Ԃ�
	return EnemyStateEnum::Attack2;
}

void EnemyCharge::OnEnter()
{
	// ���S�A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
	mTargetPos = mOwner->GetPosition() + Vector3(Math::GetRandom(-1000.0f, 1000.0f), Math::GetRandom(-1000.0f, 1000.0f), 0);
}

void EnemyCharge::OnExit()
{
}

