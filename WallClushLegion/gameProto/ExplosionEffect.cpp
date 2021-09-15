#include "ExplosionEffect.h"
#include "EffectComponent.h"

ExplosionEffect::ExplosionEffect(const Vector3& pos)
{
	mPosition = pos;
	mEffectComponent = new EffectComponent(this, true);
	mEffectComponent->LoadEffect(u"assets/Effect/01_AndrewFM01/fire.efk");

	Vector3 Pos(0, 0, 10.0f);
	mEffectComponent->SetRelativePosition(Pos);

	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(100.0f);
}

ExplosionEffect::~ExplosionEffect()
{
}
