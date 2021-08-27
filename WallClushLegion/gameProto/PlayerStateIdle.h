#pragma once
#include "PlayerStateBase.h"

class PlayerBase;

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(PlayerBehaviorComponent* owner);
	~PlayerStateIdle();

	PlayerStateEnum Update(float deltaTime) override;
	void OnEnter()override;
	void OnExit()override;

private:
	bool mIsControllerInputOff;
	bool mIsIdle;

	SkeletalMeshComponent* mMeshComp;
};