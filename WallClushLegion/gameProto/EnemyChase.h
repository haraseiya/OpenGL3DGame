#pragma once

#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class EnemyBase;

class EnemyChase: public EnemyState
{
public:
	EnemyChase(EnemyBehaviorComponent* owner,Player* player);
	virtual ~EnemyChase();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

private:
	Player* m_player;
	NPCActorBase* m_npc;
	EnemyBase* m_enemy;
};
