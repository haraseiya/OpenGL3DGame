#include "EnemyChase.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "GameScene.h"

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
{
	// �G�̃A�j���[�V������Ԃ𑖂�ɐݒ�
	mStateType = EnemyStateEnum::Run;
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	// �̗͂�0�ȉ��̏ꍇ���S��Ԃ�
	if (mOwnerActor->GetHitPoint() <= 0)
	{
		return EnemyStateEnum::Death;
	}

	// �^�[�Q�b�g�����݂��Ă��Ȃ����Idle��ԂɈڍs
	if (!mTarget)
	{
		return EnemyStateEnum::Idle;
	}

	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = mOwnerActor->GetForward();

	// �v���C���[�ւ̌��������߂�
	Vector3 enemyPos = mOwnerActor->GetPosition();
	enemyPos.z = 500;
	Vector3 npcPos = mTarget->GetPosition();
	Vector3 direction = npcPos- enemyPos;
	direction.Normalize();

	// ������x����Ă���ꍇ
	if (direction.LengthSq() > 0.5f)
	{
		// �v���C���[�̕����֌�����
		enemyPos += direction * 100.0f * deltaTime;
		mOwnerActor->SetPosition(enemyPos);

		// �����L�[����
		charaForwardVec = direction;

		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		mOwnerActor->RotateToNewForward(charaForwardVec);
	}

	// ���s
	return EnemyStateEnum::Run;
}

void EnemyChase::OnEnter()
{
	// ����A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}