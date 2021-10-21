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

	// 5�b�����Ƀv���C���[�Ɍ������Ĕ���
	mShotTimer += deltaTime;
	const bool isShot = mShotTimer > mShotInterval;
	if (isShot)
	{
		mShotTimer = 0.0f;

		// �e����
		mBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
		mBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
	}

	// ���g�̈ʒu�Ǝ��Ɍ������ʒu���擾
	Vector3 enemyPos = mOwner->GetPosition();

	// ���Ɍ������ʒu�Ɏ������ړ�������
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, 0.01f));

	return EnemyStateEnum::Attack2;
}

void EnemyCharge::OnEnter()
{
	// ���S�A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
}

void EnemyCharge::OnExit()
{
}
