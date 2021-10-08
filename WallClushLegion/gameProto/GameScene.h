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
class BulletManager;
class ThirdPersonCamera;

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
	BitMapText* mFont2;
	BulletManager* mBulletManager;
	ThirdPersonCamera* mCamera;

	static const float m_speed;
	static const float mMaxFps;
	// ê∂ê¨ç¿ïW
	float m_x;
	float m_y;
};