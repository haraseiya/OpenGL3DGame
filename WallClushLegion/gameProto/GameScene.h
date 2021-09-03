#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class Game;
class PlayerBase;
class EnemyBase;
class UIScreen;
class Texture;
class BitMapText;
class DebugGrid;
class LevelManager;
class Font;
class NPCManager;
class EnemyManager;
class SelectScene;
class FPSCounter;

class GameScene : public SceneBase
{
public:
	GameScene(PlayerBase* player);
	virtual ~GameScene();

	SceneBase* update();
	void       draw();
	void DebugLog();

	PlayerBase* GetPlayer() { return mPlayer; }


private:
	Effekseer::EffectRef mEffect;
	Effekseer::Handle    mEffectHandle;

	Game* mGame;
	PlayerBase* mPlayer;
	EnemyManager* mEnemyManager;
	SelectScene* mSelectScene;
	FPSCounter* mFPSCounter;
	DebugGrid* mGrid;
	LevelManager* mLevel;
	Texture* mTexture;
	BitMapText* mFont;

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