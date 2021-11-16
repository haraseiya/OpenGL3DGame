#pragma once

#include "EffectBase.h"

class ExplosionEffect:public EffectBase
{
public:
	ExplosionEffect();
	virtual ~ExplosionEffect();

	void LoadEffect()override;
	void CreateEffect()override;

	void SetRelativePos();
private:
	EffectComponent* mEffectComponent;
};

