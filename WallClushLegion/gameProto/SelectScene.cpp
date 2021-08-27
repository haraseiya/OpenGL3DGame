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
	// Aボタンが押されたらプレイヤー１をセット
	if (INPUT_INSTANCE.IsKeyPullup(KEY_A))
	{
		mPlayer->SetPlayerType(PlayerType::PLAYER_1);
		return new GameScene;
	}
	// Bボタンが押されたらプレイヤー２をセット
	if (INPUT_INSTANCE.IsKeyPullup(KEY_B))
	{
		mPlayer->SetPlayerType(PlayerType::PLAYER_2);
		return new GameScene;
	}

	return this;
}

void SelectScene::draw()
{
	// 背景色を設定
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
