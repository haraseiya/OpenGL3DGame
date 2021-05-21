#include "NPCManager.h"
#include "NPCActorBase.h"
#include "Attacker.h"
#include "EnemyBase.h"
#include "Math.h"

// NPC‚Ì”‰Šú‰»
const int NPCManager::m_maxNPCNum = 30;

NPCManager::NPCManager(Player* player, EnemyBase* enemy)
{
	for (int i = 0; i < m_maxNPCNum; i++)
	{
		m_npc.push_back(new Attacker(player, enemy));
		m_x= Math::GetRandom(-1000,1000);
		m_y = Math::GetRandom(-1000, 1000);
		m_npc[i]->SetPosition(Vector3(m_x, m_y, 0));
	}
}

NPCManager::~NPCManager()
{
}

void NPCManager::Update(Player* player,EnemyBase* enemy)
{
}


