#pragma once
#include "SceneBase.h"

class Texture;
class Player1;
class BitMapText;
class CameraActor;

class ResultScene:public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	SceneBase* update();
	void draw();

private:
	Texture* m_texture;
	Player1* mPlayer;

	BitMapText* mFont;
	CameraActor* mCamera;

	int mScore;
	int mLerpScore;
};

