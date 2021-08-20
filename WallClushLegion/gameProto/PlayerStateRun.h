#pragma once
#include "PlayerStateBase.h"

class Player;

class PlayerStateRun : public PlayerStateBase
{
public:
	PlayerStateRun();
	~PlayerStateRun();
	PlayerState Update(Player* owner, float deltaTime) override;
	void Enter(Player* owner, float deltaTime)override;

private:
	void MoveCalc(Player* owner, float deltaTime);
};

