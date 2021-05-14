#pragma once

#include "GameObject.h"
#include <string>

class ParticleComponent;

class ParticleBase : public GameObject
{
public:
	ParticleBase(const Vector3& pos, const Vector3& velocity, const int& lifeCount, const std::string& fileName, const float scale = 10);
	~ParticleBase();

	void UpdateActor(float deltaTime) override;
	void SetReverve(float flag);

protected:
	void LifeCountDown();

	ParticleComponent* m_particleComponent;
	Vector3 m_velocity;

	int m_lifeCount;

private:
	virtual void Update(float deltaTime);
};