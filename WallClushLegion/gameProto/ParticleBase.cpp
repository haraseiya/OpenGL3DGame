#include "ParticleBase.h"
#include 

ParticleBase::ParticleBase(const Vector3& pos, const Vector3& velocity, const int& lifeCount, const std::string& fileName, const float scale)
	: GameObject()
	, m_lifeCount(lifeCount)
	, m_velocity(velocity)
{
	SetPosition(pos);
	m_particleComponent = new ParticleComponent(this);

}

ParticleBase::~ParticleBase()
{
}

void ParticleBase::UpdateActor(float deltaTime)
{
}

void ParticleBase::SetReverve(float flag)
{
}

void ParticleBase::LifeCountDown()
{
}

void ParticleBase::Update(float deltaTime)
{
}
