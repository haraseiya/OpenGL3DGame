#include "EnemyChase.h"
#include "Player.h"
#include "NPCActorBase.h"
#include "GameScene.h"

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,NPCActorBase* npc)
	: EnemyState(owner)
	, m_npc(npc)
{
	mStateType = EnemyStateEnum::Run;
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	// NPC�����݂��Ă��Ȃ���Αҋ@�A�j���[�V�����Ɉڍs
	if (!m_npc)
	{
		return EnemyStateEnum::Idle;
	}

	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = mOwnerActor->GetForward();

	// �v���C���[�ւ̌��������߂�
	Vector3 enemyPos = mOwnerActor->GetPosition();
	Vector3 npcPos = m_npc->GetPosition();
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
		mOwnerActor->RotateToNewForward();
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