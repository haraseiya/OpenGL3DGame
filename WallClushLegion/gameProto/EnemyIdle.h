#pragma once

#include "EnemyState.h"

class EnemyBehaviorComponent;
class NPCActorBase;

class EnemyIdle :public EnemyState
{
public:
	EnemyIdle(EnemyBehaviorComponent* owner,GameObject* target);
	~EnemyIdle();

	EnemyStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

private:
	GameObject* mTarget;
};

