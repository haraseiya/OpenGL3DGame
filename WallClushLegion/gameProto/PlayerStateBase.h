#pragma once
#include "Player.h"

class PlayerStateBase
{
public:
	PlayerStateBase() {};
	virtual ~PlayerStateBase() {};
	virtual PlayerState Update(class Player* owner, float deltaTime) = 0;
	virtual void Enter(class Player* owner, float deltaTime) {};
	virtual void Exit(class Player* owner, float deltaTime) {};
};