#include "EnemyChase.h"
#include "Player.h"
#include "NPCActorBase.h"
#include "GameScene.h"

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,Player* player)
	: EnemyState(owner)
	, m_player(player)
{
	mStateType = EnemyStateEnum::Run;
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	// �A�j���[�V�����Đ��I�������������[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying()
		)
	{
		return EnemyStateEnum::Walk;
	}

	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = mOwnerActor->GetForward();

	// �v���C���[�ւ̌��������߂�
	Vector3 enemyPos = mOwnerActor->GetPosition();
	Vector3 playerPos = m_player->GetPosition();
	Vector3 direction = playerPos - enemyPos;
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
	// ���n���A�j���Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}