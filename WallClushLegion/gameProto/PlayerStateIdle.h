#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle();
	~PlayerStateIdle();
	PlayerState Update(PlayerBase* owner, float deltaTime) override;
	void Enter(PlayerBase* owner, float deltaTime)override;

private:
	bool mIsControllerInputOff;
	bool mIsIdle;

	SkeletalMeshComponent* mMeshComp;
};