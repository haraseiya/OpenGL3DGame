#pragma once
#include "Component.h"

class PlayerBase;

class InvincibleComponent : public Component
{
public:
	InvincibleComponent(PlayerBase* owner);
	~InvincibleComponent();

	void Update(float deltaTime) override;

	const bool GetIsInvicible()const { return mIsInvincible; }
	void SetIsInvicible(bool isInvicible) { mIsInvincible = isInvicible; }

private:
	PlayerBase* mOwner;

	static const int mVisibleValue;		// 見えるときの数値
	static const int mInVisibleValue;	// 見えないときの数値

	static const float mInvincibleTime;

	bool mIsInvincible;

	int mVisibleCount;
	float mInvincibleTimer;
};

