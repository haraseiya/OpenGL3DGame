#include "ExplosionEffect.h"
#include "EffectComponent.h"

// 爆発エフェクト
ExplosionEffect::ExplosionEffect()
	: EffectBase(Tag::EFFECT)
{
}

ExplosionEffect::~ExplosionEffect()
{
}

void ExplosionEffect::LoadEffect()
{
	mEffectComponent = new EffectComponent(this, true, true, false, 100);
	mEffectComponent->LoadEffect(u"assets/Effect/01_AndrewFM01/fire.efk");
}

void ExplosionEffect::CreateEffect()
{
	mEffectComponent->CreateEffect();
}

void ExplosionEffect::SetRelativePos()
{
	Vector3 Pos(0, 0, 0.0f);
	mEffectComponent->SetRelativePosition(Pos);

	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(100.0f);
}
