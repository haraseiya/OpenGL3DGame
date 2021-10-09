#pragma once

#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class PlayerBase;
class NPCActorBase;
class EnemyBehaviorComponent;
class EnemyBase;

class EnemyChase: public EnemyState
{
public:
	EnemyChase(EnemyBehaviorComponent* owner,GameObject* target);
	virtual ~EnemyChase();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	PlayerBase* mPlayer;	// �v���C���[�̃|�C���^
	GameObject* mTarget;	// �^�[�Q�b�g�̃|�C���^
	EnemyBase* mEnemy;		// �G�̃|�C���^
};
