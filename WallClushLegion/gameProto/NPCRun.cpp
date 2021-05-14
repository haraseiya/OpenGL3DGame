#include "NPCRun.h"
#include "Player.h"
#include "EnemyBase.h"
#include "GameScene.h"
#include "Input.h"

NPCRun::NPCRun(NPCBehaviorComponent* owner, Player* player, EnemyBase* enemy)
	: NPCState(owner)
	, m_mode(Mode::Chase)
	, m_player(player)
	, m_enemy(enemy)
	, m_speed(250.0f)
{
	mStateType = NPCStateEnum::Run;
}

NPCRun::~NPCRun()
{
}

NPCStateEnum NPCRun::Update(float deltaTime)
{
	// �A�j���[�V�����I�����ALookAround��ԂɈڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return NPCStateEnum::Walk;
	}

	// A�{�^���������ꂽ��A�^�b�N���[�h��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X)) m_mode = Mode::Attack;

	// B�{�^���������ꂽ��Ǐ]���[�h��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y)) m_mode = Mode::Chase;

	// ���[�h�ʂ̂ӂ�܂�
	switch (m_mode)
	{
	// �Ǐ]���[�h
	case Mode::Chase:
		// �L�����N�^�[�̑O�����擾
		m_npcForwardVec = mOwnerActor->GetForward();

		// �v���C���[�ւ̌��������߂�
		m_npcPos = mOwnerActor->GetPosition();
		m_playerPos = m_player->GetPosition();
		m_direction = m_playerPos - m_npcPos;
		m_direction.Normalize();

		// ������x����Ă���ꍇ
		if (m_direction.LengthSq() > 0.5f)
		{
			// �v���C���[�̕����֌�����
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// �����L�[����
			m_npcForwardVec = m_direction;

			// �i�s�����Ɍ����ĉ�]
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
		}
		break;

	// �U�����[�h
	case Mode::Attack:
		// NPC�̑O�����擾
		m_npcForwardVec = mOwnerActor->GetForward();

		// �v���C���[�ւ̌��������߂�
		m_npcPos = mOwnerActor->GetPosition();
		m_enemyPos = m_enemy->GetPosition();
		m_direction = m_enemyPos - m_npcPos;
		m_direction.Normalize();

		// ������x����Ă���ꍇ
		if (m_direction.LengthSq() > 0.5f)
		{
			// �v���C���[�̕����֌�����
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// �����L�[����
			m_npcForwardVec = m_direction;

			// �i�s�����Ɍ����ĉ�]
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
		}
		break;
	}

	// ���s
	return NPCStateEnum::Run;
}

void NPCRun::OnEnter()
{
	// ����A�j���Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Run);
}

void NPCRun::OnExit()
{
}