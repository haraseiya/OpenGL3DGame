#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class Game;
class PlayerBase;
class EnemyBase;
class NPCActorBase;
class NPCManager;
class UIScreen;
class Texture;
class BitMapText;
class DebugGrid;
class LevelManager;
class Font;
class NPCManager;
class EnemyManager;
class SelectScene;

class GameScene : public SceneBase
{
public:
	GameScene(PlayerBase* player);
	virtual ~GameScene();

	SceneBase* update();
	void       draw();
	void DebugLog();

	PlayerBase* GetPlayer() { return mPlayer; }
	DebugGrid* mGrid;
	LevelManager* mLevel;
	Texture* mTexture;
	BitMapText* mFont;

	Effekseer::EffectRef mEffect;
	Effekseer::Handle    mEffectHandle;

private:
	Game* m_game;
	PlayerBase* mPlayer;
	Font* m_font;
	//std::vector<WeakEnemy*> m_weakEnemy;
	EnemyBase* m_bossEnemy;
	NPCManager* mNPCManager;
	EnemyManager* mEnemyManager;
	SelectScene* mSelectScene;
	//DebugLog* m_debugLog;

	static const float m_speed;

	// NPCÇÃêî
	static const int mAttackerNum;
	static const int mClasherNum;
	static const int mHealerNum;
	static const int mAllNPCNum;

	// ê∂ë∂é“
	int m_survivalAttacker;
	int m_survivalClasher;
	int m_survivalHealer;
	int m_survivalNPC;

	// ê∂ê¨ç¿ïW
	float m_x;
	float m_y;
};