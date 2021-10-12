#pragma once
#include "Game.h"
#include <functional>

// �V�[�����
enum SceneState
{
	SCENE_TITLE = 0,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,
};

// �V�[���x�[�X�N���X
class SceneBase
{
public:
	SceneBase() {};
	virtual      ~SceneBase() {};

	virtual       SceneBase*  update() = 0;    // �V�[���̍X�V�����i�I�[�o�[���C�h�K�{�j
	virtual void  draw() = 0;                  // �V�[���̕`�揈���i�I�[�o�[���C�h�K�{�j

	SceneState GetSceneState() { return mSceneState; }

protected:
	SceneState mSceneState;
};