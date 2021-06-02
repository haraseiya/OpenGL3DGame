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
	, mRange(Vector3(100.0f,100.0f,0.0f))
{
	mStateType = NPCStateEnum::Run;
}

NPCRun::~NPCRun()
{
}

NPCStateEnum NPCRun::Update(float deltaTime)
{
	// X�{�^���������ꂽ��A�^�b�N���[�h��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X)) m_mode = Mode::Attack;
	// Y�{�^���������ꂽ��Ǐ]���[�h��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y)) m_mode = Mode::Chase;

	// �L�����N�^�[�̑O�����擾
	m_npcForwardVec = mOwnerActor->GetForward();

	// �v���C���[�ւ̌��������߂�
	m_npcPos = mOwnerActor->GetPosition();
	m_playerPos = m_player->GetPosition();
	mDistance = m_npcPos - m_playerPos;
	abs(mDistance.x);
	abs(mDistance.y);

	printf("%f", mDistance.x);
	printf("%f", mDistance.y);
	// �v���C���[��NPC�̋������߂����
	//if (mDistance.x <= 50.0f||mDistance.y<=50.0f)
	//{
	//	return NPCStateEnum::Idle;
	//}


	// ���[�h�ʂ̂ӂ�܂�
	switch (m_mode)
	{
	// �Ǐ]���[�h
	case Mode::Chase:

		m_direction = m_playerPos - m_npcPos;
		m_direction.Normalize();

			// �v���C���[�̕����֌�����
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// �����L�[����
			m_npcForwardVec = m_direction;

			// �i�s�����Ɍ����ĉ�]
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
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

			// �v���C���[�̕����֌�����
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// �����L�[����
			m_npcForwardVec = m_direction;

			// �i�s�����Ɍ����ĉ�]
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
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