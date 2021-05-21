#pragma once

#include "PlayerStateBase.h"

class PlayerStateRevive : public PlayerStateBase
{
public:
	PlayerStateRevive();
	~PlayerStateRevive();
	PlayerState Update(Player* owner, float deltaTime) override;
	void Enter(Player* owner, float deltaTime)override;
};