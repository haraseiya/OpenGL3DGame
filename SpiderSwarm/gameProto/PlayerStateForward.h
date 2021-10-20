#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateForward : public PlayerStateBase
{
public:
	PlayerStateForward(PlayerBehaviorComponent* owner);
	~PlayerStateForward();

	PlayerStateEnum Update(float deltaTime) override;

	void OnEnter()override;
	void OnExit()override;

private:
	void MoveCalc(float deltaTime);
	static const float mRoot;
	float mDot;
};

