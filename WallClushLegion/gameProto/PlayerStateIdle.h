#pragma once
#include "PlayerStateBase.h"

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle();
	~PlayerStateIdle();
	PlayerState Update(Player* owner, float deltaTime) override;
	void Enter(class Player* owner, float deltaTime)override;
};