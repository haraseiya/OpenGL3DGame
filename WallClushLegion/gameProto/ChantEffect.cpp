#include "ChantEffect.h"
#include "EffectComponent.h"

ChantEffect::ChantEffect(GameObject* owner)
	: m_lifeCount(0.0f)
{
	ec = new EffectComponent(owner, true, false,true);
	ec->LoadEffect(u"assets/Effect/MAGICALxSPIRAL/MagicArea.efk");
	Vector3 pos(0, 0, 0.1f);
	ec->SetRelativePosition(pos);
	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	ec->SetRelativeRotate(rot);
	ec->SetRelativeScale(500.0f);
}

ChantEffect::~ChantEffect()
{
}

void ChantEffect::UpdateActor(float deltaTime)
{
}
