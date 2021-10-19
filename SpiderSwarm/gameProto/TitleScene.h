#pragma once
#include "Game.h"
#include "SceneBase.h"

class Texture;
class PlayerBase;
class LevelManager;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	SceneBase* update();
	void       draw();

	class DebugGrid* mGrid;
	LevelManager* mLevel;
	PlayerBase* mPlayer;
	Texture* mTexture;
};