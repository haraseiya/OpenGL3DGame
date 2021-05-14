#pragma once
#include "PlayerStateBase.h"

class Player;

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack();
	~PlayerStateAttack();

	PlayerState Update(Player* owner, float deltaTime) override;

	void Enter(Player* owner, float deltaTime)override;
};

