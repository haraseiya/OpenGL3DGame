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
	// アニメーション終了時、LookAround状態に移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return NPCStateEnum::Walk;
	}

	// Aボタンが押されたらアタックモードに
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X)) m_mode = Mode::Attack;

	// Bボタンが押されたら追従モードに
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y)) m_mode = Mode::Chase;

	// モード別のふるまい
	switch (m_mode)
	{
	// 追従モード
	case Mode::Chase:
		// キャラクターの前方を取得
		m_npcForwardVec = mOwnerActor->GetForward();

		// プレイヤーへの向きを求める
		m_npcPos = mOwnerActor->GetPosition();
		m_playerPos = m_player->GetPosition();
		m_direction = m_playerPos - m_npcPos;
		m_direction.Normalize();

		// ある程度離れている場合
		if (m_direction.LengthSq() > 0.5f)
		{
			// プレイヤーの方向へ向かう
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// 方向キー入力
			m_npcForwardVec = m_direction;

			// 進行方向に向けて回転
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
		}
		break;

	// 攻撃モード
	case Mode::Attack:
		// NPCの前方を取得
		m_npcForwardVec = mOwnerActor->GetForward();

		// プレイヤーへの向きを求める
		m_npcPos = mOwnerActor->GetPosition();
		m_enemyPos = m_enemy->GetPosition();
		m_direction = m_enemyPos - m_npcPos;
		m_direction.Normalize();

		// ある程度離れている場合
		if (m_direction.LengthSq() > 0.5f)
		{
			// プレイヤーの方向へ向かう
			m_npcPos += m_direction * m_speed * deltaTime;
			mOwnerActor->SetPosition(m_npcPos);

			// 方向キー入力
			m_npcForwardVec = m_direction;

			// 進行方向に向けて回転
			m_npcForwardVec.Normalize();
			mOwnerActor->RotateToNewForward(m_npcForwardVec);
		}
		break;
	}

	// 続行
	return NPCStateEnum::Run;
}

void NPCRun::OnEnter()
{
	// 走りアニメ再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Run);
}

void NPCRun::OnExit()
{
}