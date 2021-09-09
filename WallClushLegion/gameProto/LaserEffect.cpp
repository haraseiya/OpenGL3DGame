#include "LaserEffect.h"
#include "EffectComponent.h"

LaserEffect::LaserEffect(const Vector3& pos)
{
	printf("�����G�t�F�N�g�`��\n");
	mPosition = pos;
	EffectComponent* ec = new EffectComponent(this, true);
	ec->LoadEffect(u"assets/Effect/01_AndrewFM01/fire.efk");

	Vector3 Pos(0, 0, 0.1f);
	ec->SetRelativePosition(Pos);
	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	ec->SetRelativeRotate(rot);
	ec->SetRelativeScale(100.0f);
}

LaserEffect::~LaserEffect()
{

}