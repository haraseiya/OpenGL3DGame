#include "SelectScene.h"
#include "Input.h"
#include "GameScene.h"
#include "PlayerBase.h"

SelectScene::SelectScene(PlayerBase* player)
	: mPlayer(player)
{

}

SelectScene::~SelectScene()
{
}

SceneBase* SelectScene::update()
{
	// A�{�^���������ꂽ��v���C���[�P���Z�b�g
	if (INPUT_INSTANCE.IsKeyPullup(KEY_A))
	{
		mPlayer->SetPlayerType(PlayerType::PLAYER_1);
		return new GameScene;
	}
	// B�{�^���������ꂽ��v���C���[�Q���Z�b�g
	if (INPUT_INSTANCE.IsKeyPullup(KEY_B))
	{
		mPlayer->SetPlayerType(PlayerType::PLAYER_2);
		return new GameScene;
	}

	return this;
}

void SelectScene::draw()
{
	// �w�i�F��ݒ�
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
