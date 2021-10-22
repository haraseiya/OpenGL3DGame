#pragma once
#include "Game.h"
#include "SceneBase.h"

class Texture;
class PlayerBase;
class EnemyBase;
class LevelManager;
class Weapon;
class CameraActor;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	SceneBase* update();
	void       draw();

private:
	class DebugGrid* mGrid;
	LevelManager* mLevel;
	PlayerBase* mPlayer;
	EnemyBase* mEnemy;
	Texture* mTexture;

	Weapon* mWeapon;
	CameraActor* mCamera;

	Vector3 mEnemyCameraOffset;
	Vector3 mPlayerCameraOffset;
	float mTimer;
};