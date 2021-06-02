#pragma once

#include "EnemyState.h"

class EnemyBehaviorComponent;
class NPCActorBase;

class EnemyIdle :public EnemyState
{
public:
	EnemyIdle(EnemyBehaviorComponent* owner,NPCActorBase* npc);
	~EnemyIdle();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

private:
	NPCActorBase* mNPC;
};

