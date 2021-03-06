#pragma once

#include "NPCState.h"
#include "NPCBehaviorComponent.h"

class Player;
class EnemyBase;

enum Mode
{
	Chase,	// Ç][h
	Attack	// U[h
};

class NPCRun : public NPCState
{
public:
	NPCRun(class NPCBehaviorComponent* owner, Player* player,EnemyBase* enemy);
	virtual ~NPCRun();
	NPCStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;
	Mode GetMode() { return m_mode; }

private:
	Player* m_player;
	EnemyBase* m_enemy;

	Vector3 m_npcForwardVec;	// NPCÌOûûüði[
	Vector3 m_npcPos;			// NPCÌÊuði[
	Vector3 m_playerPos;		// vC[ÌÊuði[
	Vector3 m_enemyPos;			// GÌÊuði[
	Vector3 m_direction;		// ü«îñði[
	Vector3 mDistance;
	Vector3 mRange;// ÍÍæèß¯êÎÒ@ÉÚs

	float m_speed;				// ¬x
	Mode m_mode;				

	float distanceX;
	float distanceY;
};
