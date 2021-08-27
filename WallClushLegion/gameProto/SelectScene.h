#pragma once
#include "SceneBase.h"

class PlayerBase;

class SelectScene : public SceneBase
{
public:
	SelectScene(PlayerBase* player);
	~SelectScene();

	SceneBase* update();
	void draw();

private:
	PlayerBase* mPlayer;
};

