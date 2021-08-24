#pragma once

class PlayerBase;

class PlayerStateBase
{
public:
	PlayerStateBase() {};
	virtual ~PlayerStateBase() {};
	virtual PlayerState Update(PlayerBase* owner, float deltaTime) = 0;
	virtual void Enter(PlayerBase* owner, float deltaTime) {};
	virtual void Exit(PlayerBase* owner, float deltaTime) {};
};