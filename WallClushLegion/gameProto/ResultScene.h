#pragma once
#include "SceneBase.h"

class Texture;

class ResultScene:public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	SceneBase* update();
	void draw();

private:
	Texture* m_texture;
};

