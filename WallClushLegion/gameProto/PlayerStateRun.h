#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun();
	~PlayerStateRun();
	PlayerState Update(PlayerBase* owner, float deltaTime) override;
	void Enter(PlayerBase* owner, float deltaTime)override;

private:
	void MoveCalc(PlayerBase* owner, float deltaTime);
	static const float mRoot;
};

