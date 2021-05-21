#pragma once
#include "GameObject.h"

class EffectBase : public GameObject
{
public:
	EffectBase();
	virtual ~EffectBase();

	void UpdateActor(float deltaTime) override;

protected:
	float m_lifeCount;
};

