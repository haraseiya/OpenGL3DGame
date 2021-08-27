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

class GameScene : public SceneBase
{
public:
	GameScene();
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
	//DebugLog* m_debugLog;

	static const float m_speed;

	// NPCの数
	static const int mAttackerNum;
	static const int mClasherNum;
	static const int mHealerNum;
	static const int mAllNPCNum;

	// 生存者
	int m_survivalAttacker;
	int m_survivalClasher;
	int m_survivalHealer;
	int m_survivalNPC;

	// 生成座標
	float m_x;
	float m_y;
};