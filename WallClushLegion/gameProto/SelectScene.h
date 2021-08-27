#pragma once
#include "SceneBase.h"

class PlayerBase;

class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();

	SceneBase* update();
	void draw();

	PlayerBase* SelectedPlayer();

private:
	std::vector<PlayerBase*> mPlayers;	// 全プレイヤーの
	PlayerBase* mSelectedPlayer;		// 選ばれたプレイヤー

	CameraActor* mCamera;
};

