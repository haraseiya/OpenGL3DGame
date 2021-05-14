#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyBehaviorComponent;

class EnemyLookAround : public EnemyState
{
public:
	EnemyLookAround(EnemyBehaviorComponent* owner);
	~EnemyLookAround();

	EnemyStateEnum Update( float deltaTime) override;

	void           OnEnter() override;
	void           OnExit()  override;

};