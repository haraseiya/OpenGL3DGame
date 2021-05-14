#pragma once
#include "GameObject.h"

class NPCActorBase;
class Player;
class EnemyBase;

class NPCManager
{
public:
	NPCManager(Player* player,EnemyBase* enemy);
	~NPCManager();

	// 更新処理
	void Update(Player* player,EnemyBase* enemy);

private:
	// クラス関連
	std::vector<NPCActorBase*> m_npc;

	static const int m_maxNPCNum;

	float m_x;
	float m_y;
	float time;
};
