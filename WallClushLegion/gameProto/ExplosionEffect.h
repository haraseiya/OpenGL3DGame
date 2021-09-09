#pragma once

#include "EffectBase.h"

class ExplosionEffect:public EffectBase
{
public:
	ExplosionEffect(const Vector3& pos);
	virtual ~ExplosionEffect();
};

