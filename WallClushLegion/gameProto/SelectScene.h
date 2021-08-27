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
	std::vector<PlayerBase*> mPlayers;	// �S�v���C���[��
	PlayerBase* mSelectedPlayer;		// �I�΂ꂽ�v���C���[

	CameraActor* mCamera;
};

