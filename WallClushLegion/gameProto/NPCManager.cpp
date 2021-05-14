#include "NPCManager.h"
#include "NPCActorBase.h"
#include "Attacker.h"
#include "EnemyBase.h"

// NPCの数初期化
const int NPCManager::m_maxNPCNum = 50;

NPCManager::NPCManager(Player* player, EnemyBase* enemy)
{
	time = 0;
}

NPCManager::~NPCManager()
{
}

void NPCManager::Update(Player* player,EnemyBase* enemy)
{
	time += 0.01;

	if (time > 3)
	{
		time = 0;
		m_npc.push_back(new Attacker(player, enemy));
	}
}


