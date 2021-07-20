#pragma once
#include "GameObject.h"

class EffectComponent;

class EffectBase : public GameObject
{
public:
	EffectBase();
	virtual ~EffectBase();

	void UpdateActor(float deltaTime) override;

protected:
	EffectComponent* mEffectComponent;
	float m_lifeCount;
};

