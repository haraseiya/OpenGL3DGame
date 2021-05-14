#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class Game;
class Player;
class WeakEnemy;
class BossEnemy;
class Attacker;
class UIScreen;
class Texture;
class BitMapText;
class DebugGrid;
class LevelManager;
class Font;
class NPCManager;

class GameScene : public SceneBase
{
public:
	GameScene();
	virtual ~GameScene();

	SceneBase* update();
	void       draw();
	void DebugLog();

	Player* GetPlayer() { return m_player; }
	DebugGrid* mGrid;
	LevelManager* mLevel;
	Texture* mTexture;
	BitMapText* mFont;

	Effekseer::EffectRef mEffect;
	Effekseer::Handle    mEffectHandle;

private:
	Game* m_game;
	Player* m_player;
	Font* m_font;
	//std::vector<WeakEnemy*> m_weakEnemy;
	BossEnemy* m_bossEnemy;
	NPCManager* m_npcManager;

	//DebugLog* m_debugLog;

	static const float m_speed;

	// NPCÇÃêî
	static const int m_attackerNum;
	static const int m_clasherNum;
	static const int m_healerNum;

	// ê∂ë∂é“
	int m_survivalAttacker;
	int m_survivalClasher;
	int m_survivalHealer;
	int m_survivalNPC;

	// ê∂ê¨ç¿ïW
	float m_x;
	float m_y;
};