#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun(PlayerBehaviorComponent* owner);
	~PlayerStateRun();
	PlayerState Update(float deltaTime) override;
	void OnEnter()override;
	void OnExit()override;

private:
	void MoveCalc(float deltaTime);
	static const float mRoot;
};

