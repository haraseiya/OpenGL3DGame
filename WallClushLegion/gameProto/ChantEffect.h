#pragma once

#include "EffectBase.h"
#include <string>

class EffectComponent;

class ChantEffect :public EffectBase
{
public:
	ChantEffect(GameObject* owner);
	virtual ~ChantEffect();

	void UpdateActor(float deltaTime) override;

	float GetLifeCount() { return m_lifeCount; }

private:
	EffectComponent* ec;
	float m_lifeCount;
};