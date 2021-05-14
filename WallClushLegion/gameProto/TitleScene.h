#pragma once
#include "Game.h"
#include "SceneBase.h"

class Texture;
class Player;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	SceneBase* update();
	void       draw();

	class DebugGrid* mGrid;
	class LevelManager* mLevel;
	Player* m_player;
	Texture* m_texture;
};